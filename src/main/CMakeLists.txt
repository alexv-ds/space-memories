
add_executable(main
  main.cpp
)

target_link_libraries(main PRIVATE core::core)
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics)
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/third_party/loose_quadtree/src/include")

add_library(lodepng
  "${CMAKE_SOURCE_DIR}/third_party/lodepng/lodepng.cpp"
)
target_include_directories(lodepng PUBLIC
  "${CMAKE_SOURCE_DIR}/third_party/lodepng"
)

target_link_libraries(main PRIVATE lodepng)
target_link_libraries(main PRIVATE stdc++fs)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
