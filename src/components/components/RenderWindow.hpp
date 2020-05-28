#pragma once

namespace component {

struct RenderWindow {
  //Изменение этих значение ничего не изменит, readonly
  unsigned int width;
  unsigned int height;
};

//Выход из программы, если будет найден этот компонент без RenderWindow
struct ExitIfWindowClosed {};

}
