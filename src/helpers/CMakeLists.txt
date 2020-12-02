add_library(helpers STATIC
  src/empty.cpp
)
add_library(helpers::helpers ALIAS helpers)

target_include_directories(helpers PUBLIC
  "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(helpers PRIVATE sfml-system sfml-window)

