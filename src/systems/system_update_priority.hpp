#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WindowEventHandler,
  ExitAfter10sec,
  RenderBegin,
  QuadRenderSystem,
  RenderEnd,
  SFMLRenderWindow,
  ExitIfWindowClosed,
  WindowEventHandlerClear,
  Init,
};


};