#pragma once

#include <typeinfo>
#include <typeindex>

namespace core {
//

using type_index = std::type_index;

template<class T>
type_index type_id() {
  return {typeid(T)};
}


}