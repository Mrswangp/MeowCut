#pragma once

#include <memory>

#include "meow_video_frame.h"
#include "proxy_include/meow_status_code.h"

namespace Meow {

struct MeowFrameBuffer {
  using Buffer = std::shared_ptr<MeowVideoFrame>;

  void Produce(Buffer buffer);

  MeowStatusCode Consume();

  MeowFrameBuffer();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_{nullptr};
};

}  // namespace Meow
