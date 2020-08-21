#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  WindowEventHandler,
  WASDRawInputMovable,
  ExitAfter10sec,
  CameraAutoPrefferedSize,
  PixelPerfectCameraSize,
  CameraFixedUnitSize,
  SpriteFrameAnimation,
  RenderBegin,
  DefaultRenderSystem,
  CameraPillarbox,
  DebugRenderRegionFrame,
  RenderEnd,
  SFMLRenderWindow,
  WindowEventHandlerClear,
  Init,
  MapLoader,
  ExitIfWindowClosed
};


};
