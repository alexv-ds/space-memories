#pragma once
namespace update_priority {

enum UpdatePriority : int {
  WindowEventHandler,
  ExitAfter10sec,
  RenderBegin,
  RenderEnd,
  SFMLRenderWindow,
  ExitIfWindowClosed,
  WindowEventHandlerClear,
  Init,
};


};