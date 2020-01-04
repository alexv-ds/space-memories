#include "ServiceLocator.hpp"

#include <unordered_map>

namespace core {

struct ServiceLocator::Impl {
  std::unordered_map<std::type_index, std::shared_ptr<IService>> map;
};

ServiceLocator::ServiceLocator(): d(std::make_unique<Impl>()) {}
ServiceLocator::~ServiceLocator() = default;

bool ServiceLocator::set_by_type_index(const std::type_index idx, const std::shared_ptr<IService> service) {
  const auto [it, insert_result] = d->map.insert({idx, service});
  return insert_result;
}
std::shared_ptr<IService> ServiceLocator::get_by_type_index(const std::type_index idx) const {
  const auto it = d->map.find(idx);
  return (it == d->map.end()) ? nullptr : it->second;
}
bool ServiceLocator::reset_by_type_index(const std::type_index idx) {
  return d->map.erase(idx) != 0;
}

} /* core */
