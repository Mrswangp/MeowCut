#pragma once

#include <cstdint>

#include "proxy_include/meow_status_code.h"

namespace Meow {

struct MeowGLUtils {
  static int32_t GenFrameBuffer();

  static int32_t GenTexture2D(uint32_t width, uint32_t height);

  static MeowStatusCode DelFrameBuffer(int32_t framebuffer);

  static MeowStatusCode DelTexture2D(int32_t texture);

  static bool HasGLContext();
};

}  // namespace Meow
