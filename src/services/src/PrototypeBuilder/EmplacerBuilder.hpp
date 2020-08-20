#pragma once

#include <core/type_index.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <functional>
#include <type_traits>
#include "ComponentEmplacer.hpp"

using json = nlohmann::json;

class IEmplacerBuilder {
public:
  virtual std::unique_ptr<IComponentEmplacer> build(const json&, core::ServiceLocator&, core::Logger&) = 0;
  virtual core::type_index component_type() = 0;
  virtual ~IEmplacerBuilder() = default;
};

template <class F>
class EmlacerBuilder final : public IEmplacerBuilder {
  using component_t = std::invoke_result_t<F, const json&, core::ServiceLocator&, core::Logger&>;
  F build_function;
public:
  EmlacerBuilder(F func): build_function(std::move(func)) {
    static_assert(std::is_invocable_v<F, const json&, core::ServiceLocator&, core::Logger&>);
  }
  core::type_index component_type() override {
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
