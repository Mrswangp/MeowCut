#pragma once

namespace Meow {

enum class MeowStatusCode {
  kOk,            ///< ok
  kInvalidParam,  ///< 非法参数
  kNonGLThread    ///< 非GL线程
};

}  // namespace Meow
