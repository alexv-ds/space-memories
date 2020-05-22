
add_library(core STATIC
  src/Core.cpp
  src/TypeRegistry.cpp
  src/ServiceLocatorImpl.cpp
)
add_library(core::core ALIAS core)

add_library(core_interface INTERFACE)
add_library(core::core_interface ALIAS core_interface)

target_include_directories(core_interface INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(core_interface INTERFACE cxx_std_17)

target_link_libraries(core PUBLIC core::core_interface)
target_link_libraries(core PUBLIC spdlog::spdlog)

