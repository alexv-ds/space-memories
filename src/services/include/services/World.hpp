#include <core/Service.hpp>
#include <entt/entity/registry.hpp>
#include <components/Position.hpp>
#include <vector>

//fabs(floatval1 - floatval2) < epsilon

namespace service {

class World : public core::Service {
public:
  struct BBox {
    BBox() = default;
    float x;
    float y;
    float size_x;
    float size_y;
  };
  virtual void query_intersects_region(std::vector<entt::entity>&, const BBox&) = 0;
  virtual void query_inside_region(std::vector<entt::entity>&, const BBox&) = 0;
  virtual void query_contains_region(std::vector<entt::entity>&, const BBox&) = 0;

  virtual void update_entity(const entt::registry&, entt::entity) = 0;
  virtual void create_body(const entt::registry&, entt::entity) = 0;
  virtual void destroy_body(const entt::registry&, entt::entity) = 0;
  virtual ~World() = default;
};


}