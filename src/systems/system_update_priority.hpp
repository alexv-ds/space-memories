#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  ListenWindowEvents,
  ImguiUpdate,
  ImguiDebugMenu,
  InputServiceUpdate,
  WASDRawInputMovable,
  DenseIntersectionControll,
  ExitAfter10sec,
  CameraAutoPrefferedSize,
  PixelPerfectCameraSize,
  CameraFixedUnitSize,
  SpaceIconInitialise,
  AddSpriteFrameeAnimationIfNeeded,
  SpriteFrameAnimation,
  ImguiContext,
  RenderBegin,
  DefaultRenderSystem,
  CameraPillarbox,
  DebugRenderRegionFrame,
  ImguiRender,
  RenderEnd,
  SFMLRenderWindow,
  InputServiceClear,
  Init,
  MapLoader,
  CloseWindowEventHandler,
  ExitIfWindowClosed
};


};
