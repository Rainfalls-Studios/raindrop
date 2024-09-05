cmake_minimum_required(VERSION 3.30)
project(Raindrop-testbed)

file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(Raindrop-Testbed ${SOURCES})

target_link_libraries(Raindrop-Testbed PUBLIC Raindrop)