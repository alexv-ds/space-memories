#include <core/define_service.hpp>

namespace core {

void register_service_builder(ServiceBuilder* builder) {
  get_registered_service_builders().push_back(builder);
}
std::vector<ServiceBuilder*>& get_registered_service_builders() {
  static std::vector<ServiceBuilder*> builders;
  return builders;
}

}
