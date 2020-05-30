#pragma once
namespace update_priority {

enum UpdatePriority : int {
  TimeUpdate,
  WindowEventHandler,
  ExitAfter10sec,
  RenderBegin,
  RenderEnd,
  SFMLRenderWindow,
  ExitIfWindowClosed,
  Init,
  WindowEventHandlerClear,
};


};