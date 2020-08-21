#pragma once

namespace component {

//C++ only
struct RenderWindow {
  //Изменение этих значение ничего не изменит, readonly
  unsigned int width;
  unsigned int height;
};

//Выход из программы, если будет найден этот компонент без RenderWindow
//C++ only
struct ExitIfWindowClosed {};

}
