#pragma once

namespace component {

//Переносит на предыдущую позицию если плотнеый объект
//Пересек другой плотный объект
struct DenseIntersectionControll {
  float old_x = 0.0f; //C++ only
  float old_y = 0.0f; //C++ only
};

}
