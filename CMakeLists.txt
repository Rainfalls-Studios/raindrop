cmake_minimum_required(VERSION 3.22.1)
project(Raindrop VERSION 0.1.0)

include(Config.cmake)

# Required by spdlog for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(SPDLOG_BUILD_STATIC ON)
set(SDL_STATIC ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(external)
add_subdirectory(utils)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)