cmake_minimum_required(VERSION 3.13)

project(SpaceMemories VERSION 0.0.1 LANGUAGES CXX)

add_executable(main
  src/main.cpp
  src/init_systems.cpp
  src/init_services.cpp

  src/core/RateLimiter.cpp
  src/core/ISystem.cpp
  src/core/ServiceLocator.cpp

  src/core/service/Shutdown.cpp
  src/core/service/Window.cpp
  src/core/service/TextureLoader.cpp
  src/core/service/LoggerFactory.cpp

  src/core/system/WindowUpdate.cpp
  src/core/system/WindowClear.cpp
  src/core/system/RenderSystem.cpp

  src/ss13/system/Init.cpp
  src/ss13/system/TickUsageCounter.cpp
)

if(NOT MSVC)
  add_compile_options(
    -Wall
    #-Wextra
    #-Wpedantic

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

target_compile_features(main PUBLIC cxx_std_17)

set(BUILD_SHARED_LIBS FALSE)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
add_subdirectory(third_party/SFML)
add_subdirectory(third_party/entt)

target_link_libraries(main EnTT::EnTT)
target_link_libraries(main sfml-system sfml-window sfml-graphics)
target_link_libraries(main termcolor::termcolor)

set(CATCH_BUILD_TESTING FALSE)
set(CATCH_ENABLE_WERROR FALSE)
set(CATCH_INSTALL_DOCS FALSE)
set(CATCH_INSTALL_HELPERS FALSE)
add_subdirectory(third_party/catch2)
add_subdirectory(third_party/termcolor)
add_executable(tests
  src/core/ServiceLocator.cpp

  tests/main.cpp
  tests/core/ServiceLocator.test.cpp
  tests/Test.test.cpp
)
target_link_libraries(tests Catch2::Catch2)
target_link_libraries(tests termcolor::termcolor)
target_include_directories(tests PRIVATE
  src
)
target_compile_features(tests PUBLIC cxx_std_17)
