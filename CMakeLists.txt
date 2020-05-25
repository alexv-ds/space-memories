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
