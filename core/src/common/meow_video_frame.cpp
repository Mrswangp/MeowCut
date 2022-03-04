#include "meow_video_frame.h"
#include "gl_manager/meow_gl_utils.h"
#include "gl_manager/meow_texture_pool.h"
#include "utils/meow_logger.h"

#include <cassert>

namespace Meow {

MeowVideoFrame::MeowVideoFrame(MeowTexture texture_in, int64_t pts_in, int64_t duration_in)
    : texture{texture_in}, pts{pts_in}, duration{duration_in} {}

MeowVideoFrame::~MeowVideoFrame() {
  assert(MeowGLUtils::HasGLContext());
  if (MeowTexturePool::Instance().GiveBack(texture) != MeowStatusCode::kOk) {
    Logger()->error("give back texture error");
  }
}

}  // namespace Meow
