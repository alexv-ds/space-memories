cmake_minimum_required(VERSION 3.13)
project(SpaceMemories VERSION 0.0.1 LANGUAGES CXX)
 
add_subdirectory(third_party/spdlog)

option(BUILD_SHARED_LIBS off)
option(SFML_BUILD_AUDIO off)
option(SFML_BUILD_NETWORK off)
add_subdirectory(third_party/SFML)

add_subdirectory(src/core)
add_subdirectory(src/main)
add_subdirectory(src/systems)
add_subdirectory(src/components)
add_subdirectory(src/services)

add_custom_target(
  resource_dir_symlink ALL COMMAND ${CMAKE_COMMAND} -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}/resources 
  ${CMAKE_CURRENT_BINARY_DIR}/resources
)
