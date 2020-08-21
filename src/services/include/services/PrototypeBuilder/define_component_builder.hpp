#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <core/define_helpers.hpp>
#include "EmplacerBuilder.hpp"

#define PROTOTYPE_DEFINE_COMPONENT_BUILDER(name, lambda)\
  namespace {\
    const ::service::ComponentBuilderDefine CORE_GENERATE_UNIQUE_NAME(component_builder_define) =\
      ::service::define_component_builder(name, std::move(lambda));\
  }

namespace service {

class ComponentBuilderDefine {
public:
  std::string name;
  std::function<std::unique_ptr<IEmplacerBuilder>()> f_emplacer_builder;

  static const std::vector<ComponentBuilderDefine*>& get_defined_builders();
  ComponentBuilderDefine(const char* name, std::function<std::unique_ptr<IEmplacerBuilder>()> func);
private:
  static std::vector<ComponentBuilderDefine*>& get_vector();
  ComponentBuilderDefine (const ComponentBuilderDefine&) = delete;
  ComponentBuilderDefine & operator = (const ComponentBuilderDefine&) = delete;
};

template <class F>
ComponentBuilderDefine define_component_builder(const char* name, F func) {
  std::function<std::unique_ptr<IEmplacerBuilder>()> f_emplacer_builder = [func]() {
    return std::make_unique<EmplacerBuilder<F>>(func);
  };
  return {name, f_emplacer_builder};
}

}
