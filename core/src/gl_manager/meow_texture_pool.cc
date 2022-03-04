#include "meow_texture_pool.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <unordered_map>

#include "meow_gl_utils.h"
#include "utils/meow_logger.h"

namespace Meow {

struct TextureSize {
  uint32_t width{0};
  uint32_t height{0};

  bool operator==(const TextureSize &size) const { return (width == size.width) && (height == size.height); }
};

struct HashTexture {
  size_t operator()(const TextureSize &size) const {
    return std::hash<uint32_t>()(size.width) ^ std::hash<uint32_t>()(size.height);
  }
};

struct AVTexture {
  uint32_t id{0};
  TextureSize size{0, 0};
  uint32_t notUseCount{0};
  bool isOccupied{false};

  explicit AVTexture(TextureSize size) {
    this->id = MeowGLUtils::GenTexture2D(size.width, size.height);
    this->size = size;
  }

  ~AVTexture() {
    if (this->id > 0) {
      MeowGLUtils::DelTexture2D(this->id);
    }
  }
};

using TextureId = int32_t;

struct MeowTexturePoolImpl {
 public:
  TextureId GetTexture(uint32_t width, uint32_t height);

  void GiveBackTexture(TextureId textureId);

  void Clear();

 private:
  void Update();

 private:
  constexpr static int32_t kTextureIdMax = 1200;
  constexpr static int32_t kTextureSizeMax = 600;
  using TextureSizeNotUseCount = uint32_t;

  std::mutex mutex_;
  std::unordered_map<TextureSize, std::list<std::shared_ptr<AVTexture>>, HashTexture> textureDetailMap_;
  std::unordered_map<TextureSize, TextureSizeNotUseCount, HashTexture> textureSizeMap_;
  std::unordered_map<TextureId, TextureSize> idSizeMap_;
};

TextureId MeowTexturePoolImpl::GetTexture(uint32_t width, uint32_t height) {
  std::lock_guard<std::mutex> lock{mutex_};
  TextureSize size{width, height};
  TextureId id;
  do {
    if (textureDetailMap_.find(size) != textureDetailMap_.end()) {
      auto &list = textureDetailMap_[size];
      /**
       * 找到没有被占用的纹理
       * */
      auto notOccupiedIt =
          std::find_if(list.begin(), list.end(), [](const std::shared_ptr<AVTexture> &t) { return !t->isOccupied; });
      if (notOccupiedIt != list.end()) {
        id = (*notOccupiedIt)->id;
        (*notOccupiedIt)->isOccupied = true;
        break;
      }
      /**
       * 没找到，需要新建一个
       * */
      auto texture = std::make_shared<AVTexture>(size);
      texture->isOccupied = true;
      id = texture->id;
      idSizeMap_[id] = size;
      list.emplace_back(std::move(texture));
      break;
    }

    /**
     * 新建纹理
     * */
    std::list<std::shared_ptr<AVTexture>> list;
    auto texture = std::make_shared<AVTexture>(size);
    texture->isOccupied = true;
    id = texture->id;
    idSizeMap_[id] = size;
    list.emplace_back(std::move(texture));
    textureDetailMap_[size] = list;
  } while (false);
  Update();
  return id;
}

void MeowTexturePoolImpl::GiveBackTexture(TextureId textureId) {
  std::lock_guard<std::mutex> lock{mutex_};
  if (idSizeMap_.find(textureId) == idSizeMap_.end()) return;
  auto size = idSizeMap_[textureId];
  if (textureDetailMap_.find(size) == textureDetailMap_.end()) return;
  auto &list = textureDetailMap_[size];
  auto it = std::find_if(list.begin(), list.end(),
                         [textureId](const std::shared_ptr<AVTexture> &t) { return t->id == textureId; });
  if (it != list.end()) {
    (*it)->isOccupied = false;
    (*it)->notUseCount = 0;
  }
}

void MeowTexturePoolImpl::Clear() {
  std::lock_guard<std::mutex> lock{mutex_};
  idSizeMap_.clear();
  textureSizeMap_.clear();
  textureDetailMap_.clear();
}

/**
 * GL环境调用
 * */
void MeowTexturePoolImpl::Update() {
  /**
   * 判断某一宽高的纹理集合是否全部没有被使用
   * */
  auto isTextureListNull = [](const std::list<std::shared_ptr<AVTexture>> &l) -> bool {
    return std::all_of(l.begin(), l.end(), [](const std::shared_ptr<AVTexture> &t) { return (!t->isOccupied); });
  };

  {
    /**
     * 一回合内没有被使用的纹理，该纹理的notUseCount会加1，否则为0
     * 一回合内没有被使用的纹理集合，该纹理集合对应的notUseCount会加1，即textureSizeMap_中的TextureSizeNotUseCount
     * */
    for (auto &item : textureDetailMap_) {
      auto &size = item.first;
      auto &list = item.second;
      if (isTextureListNull(list)) {
        textureSizeMap_[size] += 1;
      } else {
        textureSizeMap_[size] = 0;
      }
      for (auto &t : list) {
        if (!t->isOccupied) {
          t->notUseCount += 1;
        } else {
          t->notUseCount = 0;
        }
      }
    }
  }

  {
    /**
     * 清理某一类宽高的纹理集合
     * 如果某一类在kTextureSizeMax回合内都没有被使用，则清空
     * */
    auto it = textureSizeMap_.begin();
    while (it != textureSizeMap_.end()) {
      bool isDelete = false;
      auto &size = it->first;
      auto &count = it->second;
      if (count > kTextureSizeMax) {
        auto &textureList = textureDetailMap_[size];
        if (isTextureListNull(textureList)) {
          it = textureSizeMap_.erase(it);
          textureDetailMap_.erase(size);
          isDelete = true;
        }
      }
      if (!isDelete) {
        ++it;
      }
    }
  }

  {
    /**
     * 清空超过kTextureIdMax次没有被使用的纹理
     * */
    auto it = textureDetailMap_.begin();
    while (it != textureDetailMap_.end()) {
      auto &size = it->first;
      auto &detail = it->second;

      for (auto i{detail.begin()}; i != detail.end();) {
        if ((*i)->notUseCount > kTextureIdMax) {
          auto id = (*i)->id;
          idSizeMap_.erase(id);
          i = detail.erase(i);
        } else {
          ++i;
        }
      }
      ++it;
    }
  }
}

MeowTexturePool::MeowTexturePool() : impl_(new MeowTexturePoolImpl()) {}

MeowTexturePool::~MeowTexturePool() { delete impl_; }

std::optional<MeowTexture> MeowTexturePool::Get(uint32_t width, uint32_t height) {
  if (MeowGLUtils::HasGLContext()) {
    auto id = impl_->GetTexture(width, height);
    return MeowTexture(id, width, height);
  }
  return {};
}

MeowStatusCode MeowTexturePool::GiveBack(MeowTexture texture) {
  if (MeowGLUtils::HasGLContext()) {
    impl_->GiveBackTexture(texture.GetId());
    return MeowStatusCode::kOk;
  }
  Logger()->error("not gl thread");
  return MeowStatusCode::kNonGLThread;
}

MeowStatusCode MeowTexturePool::Clear() {
  if (MeowGLUtils::HasGLContext()) {
    impl_->Clear();
    return MeowStatusCode::kOk;
  }
  Logger()->error("not gl thread");
  return MeowStatusCode::kNonGLThread;
}

}  // namespace Meow
