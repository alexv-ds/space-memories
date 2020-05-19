cmake_minimum_required(VERSION 3.13)

project(SpaceMemories VERSION 0.0.1 LANGUAGES CXX)

add_executable(main
  src/main.cpp
)


target_compile_features(main PUBLIC cxx_std_17)


add_subdirectory(third_party/termcolor)

target_link_libraries(main termcolor::termcolor)
