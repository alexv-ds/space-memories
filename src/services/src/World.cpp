#include <core/define_service.hpp>
#include <core/LoggerFactory.hpp>
#include <services/World.hpp>
#include <LooseQuadtree.h>
#include <unordered_set>
#include <limits>


namespace service {

struct QuadTreeObject {
  QuadTreeObject(entt::entity holder_, const World::BBox& bbox_): holder(holder_), bbox(bbox_) {}
  entt::entity holder = entt::null;
  World::BBox bbox;
};


loose_quadtree::BoundingBox<float> create_qt_bbox(const World::BBox& bbox) {
  return {
    bbox.x - bbox.size_x * 0.5f, //left
    bbox.y - bbox.size_y * 0.5f, //top
    bbox.size_x, //width
    bbox.size_y //height
  };
}

class BBoxExtractor {
public:
  static void ExtractBoundingBox(const QuadTreeObject* object, loose_quadtree::BoundingBox<float>* bbox) {
    (*bbox) = create_qt_bbox(object->bbox);
  }
};

class WorldImpl final : public World {
  using QueryT = loose_quadtree::LooseQuadtree<float, QuadTreeObject, BBoxExtractor>::Query;
  constexpr static float epsilon = std::numeric_limits<float>::epsilon();
  std::shared_ptr<core::Logger> logger;
  std::unordered_map<entt::entity, std::unique_ptr<QuadTreeObject>> qt_objects;
  std::vector<entt::entity> remove_queue; 
  loose_quadtree::LooseQuadtree<float, QuadTreeObject, BBoxExtractor> quad_tree;
public:
  WorldImpl(std::shared_ptr<core::Logger> logger): logger(std::move(logger)) {}
  std::string_view impl_name() const noexcept override {
    return "service::WorldImpl";
  }

  World::BBox create_bbox(const component::Position& pos, const component::Body& body) {
    return World::BBox{
      pos.x,
      pos.y,
      body.size_x,
      body.size_y,
    };
  }
  
  void create_body(const entt::registry& registry, entt::entity entity) override {
    const auto [comp_pos, comp_body] = registry.get<component::Position, component::Body>(entity);
    //Надо будет как-нибудь намутить пулл QuadTreeObject объектов
    std::unique_ptr qt_object = std::make_unique<QuadTreeObject>(entity, create_bbox(comp_pos, comp_body));
    QuadTreeObject* raw_qt_object = qt_object.get();
    qt_objects.insert_or_assign(entity, std::move(qt_object));
    quad_tree.Insert(raw_qt_object);
  }

  void destroy_body(const entt::registry& registry, entt::entity entity) override {
    auto it = qt_objects.find(entity);
    if (it == qt_objects.end()) {
      return;
    }
    std::unique_ptr qt_object = std::move(it->second);
    qt_objects.erase(it);
    quad_tree.Remove(qt_object.get());
  }

  void update_qt_object(const component::Position& pos,
                        const component::Body& body,
                        QuadTreeObject* qt_object)
  {
    qt_object->bbox = create_bbox(pos, body);
    quad_tree.Update(qt_object);
  }

  void update(const entt::registry& registry) override {
    for (auto& [entity, qt_obj] : qt_objects) {
      const auto& [p_position, p_body] = registry.try_get<component::Position, component::Body>(entity);
      //Это условие возможно никогда не выполнится
      if (!p_position || !p_body) {
        continue;
      }

      World::BBox bbox = create_bbox(*p_position, *p_body);
      if ((bbox.x - qt_obj->bbox.x > epsilon) || (bbox.y - qt_obj->bbox.y > epsilon) ||
          (bbox.size_x - qt_obj->bbox.size_x > epsilon) || (bbox.size_y - qt_obj->bbox.size_y > epsilon))
      {
        update_qt_object(*p_position, *p_body, qt_obj.get());
      }
    }
  }

  //Отрефакторить копипасту
  void extract_entities(std::vector<entt::entity>& result, QueryT& query) {
    while(!query.EndOfQuery()) {
      result.push_back(query.GetCurrent()->holder);
      query.Next();
    }
  }

  void query_intersects_region(std::vector<entt::entity>& result, const BBox& region) override {
    auto qt_bbox = create_qt_bbox(region);
    auto query = quad_tree.QueryIntersectsRegion(create_qt_bbox(region));
    extract_entities(result, query);
  }
  void query_inside_region(std::vector<entt::entity>& result, const BBox& region) override {
    auto query = quad_tree.QueryInsideRegion(create_qt_bbox(region));
    extract_entities(result, query); 
  }
  void query_contains_region(std::vector<entt::entity>& result, const BBox& region) override {
    auto query = quad_tree.QueryContainsRegion(create_qt_bbox(region));
    extract_entities(result, query);  
  }
};

CORE_DEFINE_SERVICE(World, 
                   "service::World",
                   core::after<core::LoggerFactory>(),
  [](core::ServiceLocator& locator) {
    std::shared_ptr<core::LoggerFactory> logger_factory = locator.get<core::LoggerFactory>();
    return std::make_shared<WorldImpl>(logger_factory->create_logger("service::World"));
  }
);






};