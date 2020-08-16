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
  DefaultRenderSystem,
  CameraPillarbox,
  DebugRenderRegionFrame,
  RenderEnd,
  SFMLRenderWindow,
  WindowEventHandlerClear,
  Init,
  ExitIfWindowClosed
};


};
