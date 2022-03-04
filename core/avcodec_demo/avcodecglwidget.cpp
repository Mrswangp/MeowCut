#include "AVCodecGLWidget.h"

#include <qthread.h>

#include "gl_manager/meow_texture_pool.h"
#include "ui_avcodecglwidget.h"

AVCodecGLWidget::AVCodecGLWidget(QWidget *parent) : QOpenGLWidget(parent), ui(new Ui::AVCodecGLWidget) {
  ui->setupUi(this);
}

AVCodecGLWidget::~AVCodecGLWidget() {
  delete ui;
  if (generator) {
    delete generator;
  }
}

static const GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};
static const GLfloat textures[] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

#define check_error()                                                                    \
  {                                                                                      \
    int err = glGetError();                                                              \
    if (err != GL_NO_ERROR) {                                                            \
      qInfo() << "GL error" << hex << err << dec << "at" << __FILE__ << ":" << __LINE__; \
    }                                                                                    \
  }


void AVCodecGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  const char *vsrc = R"(attribute vec4 position;
                      attribute vec4 inputTextureCoordinate;
                      varying vec2 textureCoordinate;
                      void main() {
                        gl_Position = position;
                        textureCoordinate = inputTextureCoordinate.xy;
                      })";
  vshader->compileSourceCode(vsrc);

  QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  const char *fsrc = R"(varying highp vec2 textureCoordinate;
                      uniform sampler2D inputTexture;
                      void main() { 
                        gl_FragColor = texture2D(inputTexture, textureCoordinate);
                      })";

  fshader->compileSourceCode(fsrc);

  program = new QOpenGLShaderProgram;
  program->addShader(vshader);
  program->addShader(fshader);

  program->link();
  position_attribute_ = program->attributeLocation("position");
  texture_coor_attribute_ = program->attributeLocation("inputTextureCoordinate");
  input_texture_uniform_ = program->uniformLocation("inputTexture");

  generator = new Meow::MeowTextureGenerator();
  Meow::MeowStatusCode ret = generator->Init();
}

void AVCodecGLWidget::paintGL() {
#ifdef DEBUG
  qDebug() << "paintgl " << QThread::currentThreadId();
  QOpenGLContext *current = QOpenGLContext::currentContext();
  if (current) {
    qDebug() << "This is gl thread.";
  } else {
    qDebug() << "This is no gl thread.";
  }
#endif  // !DEBUG

  auto texture_opt = generator->Generate();
  if (!texture_opt.has_value()) {
    return;
  }
  auto texture = texture_opt.value();

  int w = width();
  int h = height();
  int side = qMin(w, h);
  static float flag = 2.0f;

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport((w - side) / flag, (h - side) / flag, side, side);
  flag = (flag == 2.0f) ? 3.0f : 2.0f;

  program->bind();

  glVertexAttribPointer(position_attribute_, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertices);
  glEnableVertexAttribArray(position_attribute_);

  glVertexAttribPointer(texture_coor_attribute_, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textures);
  glEnableVertexAttribArray(texture_coor_attribute_);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.GetId());

  glUniform1i(input_texture_uniform_, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(position_attribute_);
  glDisableVertexAttribArray(texture_coor_attribute_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFinish();
  Meow::MeowTexturePool::Instance().GiveBack(texture);
  program->release();
  update();
}

void AVCodecGLWidget::resizeGL(int width, int height) {}
