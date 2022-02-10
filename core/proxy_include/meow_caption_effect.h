#pragma once

#include <string>

#include "meow_common.h"
#include "meow_tracing_context.h"

namespace Meow {

struct MeowCaptionEffect {
  /**
   * @brief 设置字幕路径
   *
   * @param caption_path
   * @return MeowStatusCode
   */
  MeowStatusCode SetCaptionPath(std::string caption_path);


  ///< **************
  ///< 移动的能力
  ///< **************

  /**
   * @brief 设置具体位置
   *
   * @param position
   * @return MeowStatusCode
   */
  MeowStatusCode SetPosition(MeowPoint position);

  /**
   * @brief 设置缩放比例
   *
   * @param scale_x
   * @param scale_y
   * @param scale_z
   * @return MeowStatusCode
   */
  MeowStatusCode SetScale(float scale_x, float scale_y, float scale_z);

  /**
   * @brief 设置旋转信息
   *
   * @param rotation_x 0-360
   * @param rotation_y
   * @param rotation_z
   * @return MeowStatusCode
   */
  MeowStatusCode SetRotation(float rotation_x, float rotation_y, float rotation_z);

  struct MeowCaptionEffectImpl;
  MeowCaptionEffectImpl* impl;
};

}  // namespace Meow
