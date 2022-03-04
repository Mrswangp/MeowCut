#pragma once

#include <memory>

#include "common/meow_texture.h"
#include "proxy_include/meow_status_code.h"

namespace Meow {

struct MeowBasicRender {
 public:
  MeowBasicRender();

  MeowStatusCode Init();

  MeowStatusCode UnInit();

  MeowStatusCode Render(MeowTexture texture_in, MeowTexture texture_out);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_{nullptr};
};

}  // namespace Meow
