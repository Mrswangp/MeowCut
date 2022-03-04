#include "meow_gl_context.h"

#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QSurface>
#include <QThread>
#include <QWindow>
#include <atomic>
#include <cassert>
#include <functional>

#include "meow_gl_utils.h"

namespace Meow {

static QOpenGLContext* MeowGLCurrentContext() { return QOpenGLContext::currentContext(); }

static QOpenGLFunctions* MeowGLFunctions() {
  QOpenGLContext* context = MeowGLCurrentContext();
  if (context) {
    return context->functions();
  }
  return nullptr;
}

struct MeowEGLCoreThread : QThread {
 public:
  using Func = std::function<void()>;

  MeowEGLCoreThread(Func func) : run_func_{func} {}

  MeowEGLCoreThread(Func func, QOpenGLContext* qcontext, QObject* parent) : run_func_{func}, QThread(parent) {
    InitEGL(qcontext);
  }

  MeowEGLCoreThread(Func func, QOpenGLContext* qcontext) : run_func_{func}, QThread(nullptr) { InitEGL(qcontext); }

  ~MeowEGLCoreThread() { wait(); }

  MeowEGLThread::Context GetContext() { return reinterpret_cast<MeowEGLThread::Context>(context_.get()); }

  void run() override {
    context_->makeCurrent(static_cast<QSurface*>(surface_.get()));
    if (run_func_) {
      run_func_();
    }
  }

 private:
  void InitEGL(QOpenGLContext* qcontext) {
    assert(!MeowGLUtils::HasGLContext());
    assert(context_ == nullptr);
    assert(surface_ == nullptr);
    surface_ = std::make_unique<QOffscreenSurface>();
    surface_->setFormat(qcontext->format());
    surface_->create();
    context_ = std::make_unique<QOpenGLContext>();
    context_->setFormat(qcontext->format());
    context_->setShareContext(qcontext);
    context_->create();
    context_->moveToThread(this);
  }

 private:
  std::unique_ptr<QOpenGLContext> context_{nullptr};
  std::unique_ptr<QOffscreenSurface> surface_{nullptr};
  Func run_func_{nullptr};
};

MeowEGLThread::MeowEGLThread(Func func) : egl_core_{std::make_unique<MeowEGLCoreThread>(func)} {}

MeowEGLThread::~MeowEGLThread() {}

}  // namespace Meow
