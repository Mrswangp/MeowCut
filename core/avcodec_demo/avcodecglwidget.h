#ifndef AVCODECGLWIDGET_H
#define AVCODECGLWIDGET_H

#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include "gl_manager/meow_gen_texture.h"

namespace Ui {
class AVCodecGLWidget;
}

class AVCodecGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit AVCodecGLWidget(QWidget *parent = nullptr);
  ~AVCodecGLWidget();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

 private:
  Ui::AVCodecGLWidget *ui;

 private:
  QOpenGLShaderProgram *program;
  GLuint position_attribute_{0};
  GLuint texture_coor_attribute_{0};
  GLuint input_texture_uniform_{0};
  Meow::MeowTextureGenerator *generator{nullptr};
};

#endif  // AVCODECGLWIDGET_H
