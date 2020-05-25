#include <core/Service.hpp>

namespace core {

std::string_view Service::impl_name() const noexcept {
  return "***UNNAMED***";
}
 
}