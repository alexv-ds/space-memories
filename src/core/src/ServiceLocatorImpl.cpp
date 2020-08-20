#include <cassert>
#include <core/define_service.hpp>
#include "ServiceLocatorImpl.hpp"


namespace core {

std::vector<ServiceDefine*> create_sorted_defines(const std::vector<ServiceDefine*>& unsorted);

ServiceLocatorImpl::ServiceLocatorImpl(std::shared_ptr<Logger> logger): logger(logger) {}

std::shared_ptr<Service> ServiceLocatorImpl::try_get(type_index type) noexcept {
  auto it = services.find(type);
  return it != services.end() ? it->second : nullptr;
}

void ServiceLocatorImpl::add_service(std::string_view name, type_index type, std::shared_ptr<Service> service) {
  assert(service != nullptr);
  auto it = services.find(type);
  if (it != services.end()) {
    logger->error("Двойная регистрация сервиса {}. Impl: {}", name, service->impl_name());
    return;
  }

  services.insert({type, service});
  logger->trace("Сервис {} зарегестрирован. Impl: {}", name, service->impl_name());
}

void ServiceLocatorImpl::init_service_defines() {
  std::vector<ServiceDefine*> defined_services = create_sorted_defines(ServiceDefine::get_defined_services());
  for (const ServiceDefine* service_define : defined_services) {
    try {
      logger->trace("Инициализация {}", service_define->name);
      std::shared_ptr service = service_define->build_func(*this);
      if (service == nullptr) {
        logger->error(
          "Не удалось создать сервис {}, функция создания вернула nullptr", service_define->name
        );
        continue;
      }
      add_service(service_define->name, service_define->interface, std::move(service));
    } catch (std::exception& e) {
      logger->error("Не удалось создать сервис {}: {}", service_define->name, e.what());
    }
  }
}

struct VectorElem {
  ServiceDefine* define;
  bool visited;
};

void push_with_all_deps(VectorElem& elem,
                   std::vector<VectorElem>& defines,
                   std::vector<ServiceDefine*>& sorted_defines)
{
  if (elem.visited) {
    return;
  }
  elem.visited = true;
  for (type_index type : elem.define->after) {
    for (VectorElem& elem : defines) {
      if (elem.define->interface == type) {
        push_with_all_deps(elem, defines, sorted_defines);
      }
    }
  }
  sorted_defines.push_back(elem.define);
}

std::vector<ServiceDefine*> create_sorted_defines(const std::vector<ServiceDefine*>& unsorted) {
  //Возможно стоит пересмотреть решение N^2 сложность
  std::vector<ServiceDefine*> sorted_defines;
  sorted_defines.reserve(unsorted.size());
  std::vector<VectorElem> defines;
  defines.reserve(unsorted.size());
  for (ServiceDefine* define : unsorted) {
    defines.push_back({define, false});
  }
  for (VectorElem& elem : defines) {
    push_with_all_deps(elem, defines, sorted_defines);
  }
  return sorted_defines;
}

}
