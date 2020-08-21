#include <services/PrototypeBuilder/define_component_builder.hpp>

namespace service {

ComponentBuilderDefine::ComponentBuilderDefine(const char* name,
                                               std::function<std::unique_ptr<IEmplacerBuilder>()> func):
    name(name),
    f_emplacer_builder(std::move(func))
{
  get_vector().push_back(this);
}


std::vector<ComponentBuilderDefine*>& ComponentBuilderDefine::get_vector() {
  static std::vector<ComponentBuilderDefine*> builders;
  return builders;
}

const std::vector<ComponentBuilderDefine*>& ComponentBuilderDefine::get_defined_builders() {
  return get_vector();
}

}
