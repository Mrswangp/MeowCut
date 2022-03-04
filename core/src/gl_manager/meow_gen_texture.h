#pragma once

#include <memory>
#include <optional>

#include "common/meow_texture.h"
#include "proxy_include/meow_status_code.h"

namespace Meow {

struct MeowTextureGenerator {
 public:
  MeowTextureGenerator();

  ~MeowTextureGenerator();

  MeowStatusCode Init();

  MeowStatusCode UnInit();

  std::optional<MeowTexture> Generate();

 private:
  struct Impl;
  Impl* impl_{nullptr};
};

}  // namespace Meow
