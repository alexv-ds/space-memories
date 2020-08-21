#pragma once

#include <memory>
#include <functional>
#include <type_traits>
#include <core/type_index.hpp>
#include <core/ServiceLocator.hpp>
#include <nlohmann/json.hpp>
#include "ComponentEmplacer.hpp"

class IEmplacerBuilder {
protected:
  using json = nlohmann::json;
public:
  virtual std::unique_ptr<IComponentEmplacer> build(const json&, core::ServiceLocator&, core::Logger&) = 0;
  virtual core::type_index component_type() const = 0;
  virtual ~IEmplacerBuilder() = default;
};

template <class F>
class EmplacerBuilder final : public IEmplacerBuilder {
  using component_t = std::invoke_result_t<F, const json&, core::ServiceLocator&, core::Logger&>;
  F build_function;
public:
  EmplacerBuilder(F func): build_function(std::move(func)) {
    static_assert(std::is_invocable_v<F, const json&, core::ServiceLocator&, core::Logger&>);
  }
  core::type_index component_type() const override {
    return core::type_id<component_t>();
  }

  std::unique_ptr<IComponentEmplacer> build(const json& json,
                                            core::ServiceLocator& locator,
                                            core::Logger& logger) override
  {
    component_t compoent = build_function(json, locator, logger);
    return std::make_unique<ComponentEmplacer<component_t>>(compoent);
  }
};
