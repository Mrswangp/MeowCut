#pragma once


#include "meow_audio_layer_group.h"
#include "meow_canvas_layer.h"
#include "meow_editor_layer.h"
#include "meow_effect_layer_group.h"
#include "meow_engine_controller.h"
#include "meow_pip_layer_group.h"
#include "meow_unit_factory.h"

namespace Meow {

struct MeowEngine {
  /**
   * @brief 获取链路追踪上下文
   *
   * @return MeowTracingContext*
   */
  static MeowTracingContext* GetTracingContext();

  /**
   * @brief 整个Engine作用前首先调用此接口进行配置，然后才能得到相应的图层
   *
   * @return MeowStatusCode
   */
  MeowStatusCode Configure(MeowVideoConfig video_config, MeowAudioConfig audio_config);

  /**
   * @brief 获取画布图层
   *
   * @return MeowCanvasLayer*
   */
  MeowCanvasLayer* GetCanvasLayer();

  /**
   * @brief 获取编辑图层
   *
   * @return MeowEditorLayer*
   */
  MeowEditorLayer* GetEditorLayer();

  /**
   * @brief 获取音频图层组
   *
   * @return MeowAudioLayerGroup*
   */
  MeowAudioLayerGroup* GetAudioLayerGroup();

  /**
   * @brief 获取画中画图层组
   *
   * @return MeowPipLayerGroup*
   */
  MeowPipLayerGroup* GetPipLayerGroup();

  /**
   * @brief 获取特效图层组
   *
   * @return MeowEffectLayerGroup*
   */
  MeowEffectLayerGroup* GetEffectLayerGroup();

  /**
   * @brief 获取Unit工厂，具体见MeowUnitFactory
   *
   * @return MeowUnitFactory*
   */
  MeowUnitFactory* GetUnitFactory();

  /**
   * @brief 获取Engine的控制器
   *
   * @return MeowEngineController*
   */
  MeowEngineController* GetController();
};

}  // namespace Meow
