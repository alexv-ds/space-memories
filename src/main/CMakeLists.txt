
add_executable(main
  main.cpp
)

target_link_libraries(main PRIVATE core::core)
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics)