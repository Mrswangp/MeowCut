#include "meow_gl_utils.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace Meow {

static QOpenGLContext* MeowGLCurrentContext() { return QOpenGLContext::currentContext(); }

static QOpenGLFunctions* MeowGLFunctions() {
  QOpenGLContext* context = MeowGLCurrentContext();
  if (context) {
    return context->functions();
  }
  return nullptr;
}

bool MeowGLUtils::HasGLContext() { return MeowGLCurrentContext() != nullptr; }

int32_t MeowGLUtils::GenFrameBuffer() {
  QOpenGLFunctions* functions = MeowGLFunctions();
  if (!functions) return -1;
  GLuint framebuffer = 0;
  functions->glGenFramebuffers(1, &framebuffer);
  return framebuffer;
}

int32_t MeowGLUtils::GenTexture2D(uint32_t width, uint32_t height) {
  QOpenGLFunctions* functions = MeowGLFunctions();
  if (!functions) return -1;
  GLuint texture = 0;
  functions->glGenTextures(1, &texture);
  functions->glBindTexture(GL_TEXTURE_2D, texture);
  functions->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  functions->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  functions->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  functions->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  functions->glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

MeowStatusCode MeowGLUtils::DelFrameBuffer(int32_t framebuffer) {
  QOpenGLFunctions* functions = MeowGLFunctions();
  if (!functions) return MeowStatusCode::kNonGLThread;
  GLuint f = static_cast<GLuint>(framebuffer);
  functions->glDeleteFramebuffers(1, &f);
  return MeowStatusCode::kOk;
}

MeowStatusCode MeowGLUtils::DelTexture2D(int32_t texture) {
  QOpenGLFunctions* functions = MeowGLFunctions();
  if (!functions) return MeowStatusCode::kNonGLThread;
  GLuint t = static_cast<GLuint>(texture);
  functions->glDeleteTextures(1, &t);
  return MeowStatusCode::kOk;
}

}  // namespace Meow
