cmake_minimum_required(VERSION 3.22.1)
project(Raindrop VERSION 0.1.0)

include(Config.cmake)

add_subdirectory(external)
add_subdirectory(utils)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)