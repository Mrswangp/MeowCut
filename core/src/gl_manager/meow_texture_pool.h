#pragma once

#include <memory>
#include <optional>

#include "common/meow_texture.h"
#include "proxy_include/meow_status_code.h"

namespace Meow {
struct MeowTexturePoolImpl;
struct MeowTexturePool {
  static MeowTexturePool& Instance() {
    static MeowTexturePool t;
    return t;
  }

  /**
   * @brief GL线程调用
   *
   * @param width
   * @param height
   * @return MeowTexture
   */
  std::optional<MeowTexture> Get(uint32_t width, uint32_t height);

  /**
   * @brief GL线程调用
   *
   * @param texture
   * @return MeowStatusCode
   */
  MeowStatusCode GiveBack(MeowTexture texture);

  MeowStatusCode Clear();

 private:
  MeowTexturePool();

  ~MeowTexturePool();

  MeowTexturePoolImpl* impl_{nullptr};
};

}  // namespace Meow
