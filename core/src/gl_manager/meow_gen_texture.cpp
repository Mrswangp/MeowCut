#include "meow_gen_texture.h"

#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <vector>

#include "meow_gl_utils.h"
#include "meow_texture_pool.h"

namespace Meow {

struct MeowTextureGenerator::Impl {
  MeowStatusCode Init() {
    if (!MeowGLUtils::HasGLContext()) {
      return MeowStatusCode::kNonGLThread;
    }
    images_.resize(5);
    images_[0].load("C:/Users/A/Desktop/image/meow.png");
    images_[1].load("C:/Users/A/Desktop/image/cat.jpg");
    images_[2].load("C:/Users/A/Desktop/image/cxy.jpg");
    images_[3].load("C:/Users/A/Desktop/image/p.jpg");
    images_[4].load("C:/Users/A/Desktop/image/q.jpg");
    images_[0] = images_[0].convertToFormat(QImage::Format_RGBA8888);
    images_[1] = images_[1].convertToFormat(QImage::Format_RGBA8888);
    images_[2] = images_[2].convertToFormat(QImage::Format_RGBA8888);
    images_[3] = images_[3].convertToFormat(QImage::Format_RGBA8888);
    images_[4] = images_[4].convertToFormat(QImage::Format_RGBA8888);
    return MeowStatusCode::kOk;
  }

  MeowStatusCode UnInit() {
    images_.clear();
    return MeowStatusCode::kOk;
  }

  QImage& GetImage() {
    static int32_t index = 0;
    ++index;
    if (index == images_.size()) {
      index = 0;
    }
    return images_[index];
  }

  static QOpenGLContext* MeowGLCurrentContext() { return QOpenGLContext::currentContext(); }

  static QOpenGLFunctions* MeowGLFunctions() {
    QOpenGLContext* context = MeowGLCurrentContext();
    if (context) {
      return context->functions();
    }
    return nullptr;
  }

  std::optional<MeowTexture> Generate() {
    QImage& image = GetImage();
    int32_t width = image.width();
    int32_t height = image.height();
    auto& texture_pool = MeowTexturePool::Instance();
    auto texture_opt = texture_pool.Get(width, height);
    if (!texture_opt.has_value()) return {};

    QOpenGLFunctions* f = MeowGLFunctions();
    auto texture = texture_opt.value();
    glBindTexture(GL_TEXTURE_2D, texture.GetId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
  }

 private:
  std::vector<QImage> images_;
};

MeowTextureGenerator::MeowTextureGenerator() : impl_{new Impl()} {}

MeowTextureGenerator::~MeowTextureGenerator() { delete impl_; }

MeowStatusCode MeowTextureGenerator::Init() { return impl_->Init(); }

MeowStatusCode MeowTextureGenerator::UnInit() { return impl_->UnInit(); }

std::optional<MeowTexture> MeowTextureGenerator::Generate() { return impl_->Generate(); }

}  // namespace Meow
