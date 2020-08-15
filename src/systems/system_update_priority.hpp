#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  WindowEventHandler,
  WASDRawInputMovable,
  ExitAfter10sec,
  PixelPerfectCameraSize,
  RenderBegin,
  QuadRenderSystem,
  CameraPillarbox,
  RenderEnd,
  SFMLRenderWindow,
  WindowEventHandlerClear,
  Init,
  ExitIfWindowClosed,
};


};
