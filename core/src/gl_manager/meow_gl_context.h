#pragma once

#include <functional>
#include <memory>

namespace Meow {

struct MeowEGLCoreThread;

struct MeowEGLThread {
  using Context = void*;
  using AVNativeWindow = void*;
  using Surface = AVNativeWindow;
  using Func = std::function<void()>;

  explicit MeowEGLThread(Func func);

  ~MeowEGLThread();

 private:
  std::unique_ptr<MeowEGLCoreThread> egl_core_{nullptr};

  Context context_{nullptr};
};

}  // namespace Meow
