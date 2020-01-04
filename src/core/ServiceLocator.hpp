#pragma once

#include <typeindex>
#include <memory>
#include <core/IService.hpp>

namespace core {

class ServiceLocator {
public:
  template <typename I, typename T>
  inline bool set(std::shared_ptr<T>);

  template <typename T>
  inline std::shared_ptr<T> get() const;

  template <typename T>
  inline bool reset();

  ServiceLocator();
  ~ServiceLocator();

private:
  struct Impl;
  std::unique_ptr<Impl> d;

  ServiceLocator(const ServiceLocator&) = delete;
  void operator=(const ServiceLocator&) = delete;

  bool set_by_type_index(const std::type_index, const std::shared_ptr<IService>);
  std::shared_ptr<IService> get_by_type_index(const std::type_index) const;
  bool reset_by_type_index(const std::type_index);
};


//Inline methods
template <typename I, typename T>
inline bool ServiceLocator::set(std::shared_ptr<T> p_service) {
  return set_by_type_index(
    std::type_index(typeid(I)),
    //Сначала кастуем в I, потом в IService
    std::static_pointer_cast<IService>(std::static_pointer_cast<I>(p_service))
  );
}

template <typename T>
inline std::shared_ptr<T> ServiceLocator::get() const {
  return std::static_pointer_cast<T>(
    get_by_type_index( std::type_index(typeid(T)) )
  );
}

template <typename T>
inline bool ServiceLocator::reset() {
  return reset_by_type_index( std::type_index(typeid(T)) );
}


} /* core */
