#pragma once

#include <cstdint>

#include "meow_texture.h"

namespace Meow {

struct MeowVideoFrame {
  MeowVideoFrame(MeowTexture texture_in, int64_t pts_in, int64_t duration_in);
  ~MeowVideoFrame();

  int64_t pts{0};
  int64_t duration{0};

  uint8_t* data{nullptr};  ///< 裸数据 RGBA格式
  int32_t data_size{0};

  MeowTexture texture;
};

}  // namespace Meow
