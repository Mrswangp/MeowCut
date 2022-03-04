#include "meow_frame_buffer.h"

#include <QOpenGLFunctions>
#include <mutex>
#include <queue>

#include "gl_manager/meow_gl_utils.h"

namespace Meow {

struct MeowFrameBuffer::Impl {
  using Buffer = MeowFrameBuffer::Buffer;

  void Produce(Buffer buffer);

  MeowStatusCode Consume();

 private:
  std::queue<Buffer> buffer_queue_;
  std::mutex mutex_;
};

static QOpenGLContext* MeowGLCurrentContext() { return QOpenGLContext::currentContext(); }

static QOpenGLFunctions* MeowGLFunctions() {
  QOpenGLContext* context = MeowGLCurrentContext();
  if (context) {
    return context->functions();
  }
  return nullptr;
}

void MeowFrameBuffer::Impl::Produce(Buffer buffer) {
  std::lock_guard<std::mutex> lock(mutex_);
  buffer_queue_.push(buffer);
}

MeowStatusCode MeowFrameBuffer::Impl::Consume() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!MeowGLUtils::HasGLContext()) {
    return MeowStatusCode::kNonGLThread;
  }
  if (buffer_queue_.empty()) return MeowStatusCode::kInvalidParam;
  {
    Buffer buffer = buffer_queue_.front();
    buffer_queue_.pop();

    QOpenGLFunctions* f = MeowGLFunctions();
    f->glBindTexture(GL_TEXTURE_2D, buffer->texture.GetId());

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    f->glActiveTexture(GL_TEXTURE0);

    f->glDrawArrays(GL_TRIANGLES, 0, 4);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    f->glFinish();
  }
  return MeowStatusCode::kOk;
}

MeowFrameBuffer::MeowFrameBuffer() : impl_{std::make_unique<Impl>()} {}

void MeowFrameBuffer::Produce(Buffer buffer) { impl_->Produce(buffer); }

MeowStatusCode MeowFrameBuffer::Consume() { return impl_->Consume(); }

}  // namespace Meow
