#include "ServiceLocatorImpl.hpp"

namespace core {

ServiceLocatorImpl::ServiceLocatorImpl(std::shared_ptr<TypeRegistry> type_registry,
                                       std::shared_ptr<Logger> logger):
                          //initialise
                          ServiceLocator(std::move(type_registry)),
                          logger(logger)
{
  //Загрузка builtin сервисов
}

std::shared_ptr<Service> ServiceLocatorImpl::try_get(type_index type) noexcept {
  auto it = services.find(type);
  return it != services.end() ? it->second : nullptr;
}

void ServiceLocatorImpl::add_service(type_index type, std::shared_ptr<Service> service) {
  auto it = services.find(type);
  if (!service) {
    logger->warn("Передан нулевой указатель для сервиса {}", type_registry->get_name(type).first);
    return;
  }
  if (it != services.end()) {
    logger->error("Двойная регистрация сервиса {}", type_registry->get_name(type).first);
    return;
  }
  services.insert({type, std::move(service)});
  logger->trace("Сервис {} зарегестрирован", type_registry->get_name(type).first);
}

}