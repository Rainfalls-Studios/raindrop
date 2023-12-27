cmake_minimum_required(VERSION 3.12.0)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

 # ignore BOOST deprecated headers
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

find_package(Boost REQUIRED COMPONENTS Filesystem)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(Raindrop STATIC ${SOURCES})

add_subdirectory(libs)

target_include_directories(Raindrop PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(Raindrop
	PRIVATE
	Boost::filesystem
	Vulkan::Vulkan
	
	PUBLIC
	glm_static
	yaml-cpp::yaml-cpp
	spdlog::spdlog
	muparser::muparser
	SDL2::SDL2-static
	SDL2::SDL2main
)

add_subdirectory(tests)