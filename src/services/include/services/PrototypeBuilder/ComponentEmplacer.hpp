#pragma once
#include <entt/entity/registry.hpp>

class IComponentEmplacer {
public:
  virtual void emplace(entt::entity, entt::registry&) = 0;
  virtual ~IComponentEmplacer() = default;
};

template <class T>
class ComponentEmplacer final : public IComponentEmplacer {
public:
  ComponentEmplacer(T& component): component(component) {}
  void emplace(entt::entity entity, entt::registry& registry) override {
    registry.emplace_or_replace<T>(entity, component);
  }
private:
  T component;
};
