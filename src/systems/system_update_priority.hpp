#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WorldUpdate,
  WindowEventHandler,
  WASDRawInputMovable,
  ExitAfter10sec,
  RenderBegin,
  QuadRenderSystem,
  CameraPillarbox,
  RenderEnd,
  SFMLRenderWindow,
  ExitIfWindowClosed,
  WindowEventHandlerClear,
  Init,
};


};
