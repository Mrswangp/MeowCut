#include "meow_basic_render.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "gl_manager/meow_gl_utils.h"

namespace Meow {

const char *vsrc = R"(attribute vec4 position;
                      attribute vec4 inputTextureCoordinate;
                      varying vec2 textureCoordinate;
                      void main() {
                        gl_Position = position;
                        textureCoordinate = inputTextureCoordinate.xy;
                      })";

const char *fsrc = R"(varying highp vec2 textureCoordinate;
                      uniform sampler2D inputTexture;
                      void main() { 
                        vec2 temp = textureCoordinate.xy;
                        float x;
                        if (temp.x > 0.0 && temp.x < 0.5) {
                          x = temp.x + 0.25;
                        } else {
                          x = temp.x - 0.25;
                        }
                        vec4 mask = texture2D(inputTexture, vec2(x, temp.y));
                        gl_FragColor = vec4(mask.rgb, 1.0);
                      })";

static const GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};
static const GLfloat textures[] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

struct MeowBasicRender::Impl : QOpenGLFunctions {
  MeowStatusCode Init() {
    if (!MeowGLUtils::HasGLContext()) {
      return MeowStatusCode::kNonGLThread;
    }
    initializeOpenGLFunctions();

    program_ = std::make_unique<QOpenGLShaderProgram>();
    program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    program_->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    program_->link();

    position_attribute_ = program_->attributeLocation("position");
    texture_coor_attribute_ = program_->attributeLocation("inputTextureCoordinate");
    input_texture_uniform_ = program_->uniformLocation("inputTexture");
    return MeowStatusCode::kOk;
  }

  MeowStatusCode UnInit() {
    if (!MeowGLUtils::HasGLContext()) {
      return MeowStatusCode::kNonGLThread;
    }
    return MeowStatusCode::kOk;
  }

  MeowStatusCode Render(MeowTexture texture_in, MeowTexture texture_out) {
    if (!MeowGLUtils::HasGLContext()) {
      return MeowStatusCode::kNonGLThread;
    }
    program_->bind();
    GLuint fbo;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE_2D, texture_out.GetId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_out.GetId(), 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, texture_out.GetWidth(), texture_out.GetHeight());

    glVertexAttribPointer(position_attribute_, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(position_attribute_);

    glVertexAttribPointer(texture_coor_attribute_, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textures);
    glEnableVertexAttribArray(texture_coor_attribute_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_in.GetId());

    glUniform1i(input_texture_uniform_, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(position_attribute_);
    glDisableVertexAttribArray(texture_coor_attribute_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glFinish();
    program_->release();
    return MeowStatusCode::kOk;
  }

 private:
  std::unique_ptr<QOpenGLShaderProgram> program_{nullptr};
  GLuint position_attribute_{0};
  GLuint texture_coor_attribute_{0};
  GLuint input_texture_uniform_{0};
};

MeowBasicRender::MeowBasicRender() : impl_{std::make_unique<Impl>()} {}

MeowStatusCode MeowBasicRender::Init() { return impl_->Init(); }

MeowStatusCode MeowBasicRender::UnInit() { return impl_->UnInit(); }

MeowStatusCode MeowBasicRender::Render(MeowTexture texture_in, MeowTexture texture_out) {
  return impl_->Render(texture_in, texture_out);
}

}  // namespace Meow
