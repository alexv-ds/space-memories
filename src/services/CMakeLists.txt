target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(main PRIVATE
  src/SFMLRenderWindow.cpp
  src/Input.cpp
  src/TimeImpl.cpp
  src/Camera.cpp
  src/World.cpp

  src/SpriteManager/SpriteManager.cpp
  src/SpriteManager/DmiMetadata.cpp

  src/PrototypeBuilder/PrototypeBuilder.cpp
  src/PrototypeBuilder/define_component_builder.cpp
)
