#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <core/Logger.hpp>

class ComponentBuilderRegistry {
  std::shared_ptr<core::Logger> logger;
  //std::unordered_map<std::string name, std::unique_ptr<IEmplacerBuilder>> emplacer_builders;

public:
  ComponentBuilderRegistry(std::shared_ptr<core::Logger> logger): logger(std::move(logger)) {}

  template <class F>
  void add_builder(std::string_view component_name, F builder) {
    std::unique_ptr<IEmplacerBuilder> emplacer_builder =
      std::make_unique<EmlacerBuilder<F>>(std::move(builder));
    //
    

    return;
  }
};
