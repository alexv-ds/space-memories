cmake_minimum_required(VERSION 3.13)

project(SpaceMemories VERSION 0.0.1 LANGUAGES CXX)

add_executable(main
  src/main.cpp
  src/init_systems.cpp
  src/init_services.cpp

  src/core/RateLimiter.cpp
  src/core/ISystem.cpp

  src/core/service/Shutdown.cpp

  src/ss13/system/Init.cpp
)

if(NOT MSVC)
  target_compile_options(main PRIVATE
    -Wall
    #-Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    #-Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    #-Wsign-conversion
    -Wsign-promo
  )
endif()


target_include_directories(main PRIVATE
  src
)

target_compile_features(main PUBLIC cxx_std_20)

set(BUILD_SHARED_LIBS FALSE)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
add_subdirectory(third_party/SFML)
add_subdirectory(third_party/entt)

target_link_libraries(main EnTT::EnTT)
target_link_libraries(main sfml-system sfml-window sfml-graphics)
