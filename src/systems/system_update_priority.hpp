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
  RenderBegin,
  QuadRenderSystem,
  CameraPillarbox,
  DebugRenderRegionFrame,
  RenderEnd,
  SFMLRenderWindow,
  WindowEventHandlerClear,
  Init,
  ExitIfWindowClosed,
};


};
