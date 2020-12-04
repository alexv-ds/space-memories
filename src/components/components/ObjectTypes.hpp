#pragma once
#include <string>

namespace component {

//Плотный объект, через него не сможет пройти другой плотный объект
struct DenseObject {};

//вешается при создании через PrototypeBuilder
//C++ only
struct Prototype {
  std::string name;
};

}
