cmake_minimum_required(VERSION 3.12.0)
project(Raindrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

 # ignore BOOST deprecated headers
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

find_package(Boost REQUIRED COMPONENTS Filesystem)
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(Raindrop STATIC ${SOURCES})

add_subdirectory(libs)

target_include_directories(Raindrop PRIVATE
	${BOOST_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${SPDLOG_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
	${yaml-cpp_INCLUDE_DIRS}
	${muparser_INCLUDE_DIRS}
)

target_include_directories(Raindrop PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(Raindrop PRIVATE
	Boost::filesystem
	spdlog::spdlog
	glm::glm
	Vulkan::Vulkan
	shaderc_combined
	SDL2::SDL2
	yaml-cpp::yaml-cpp
	muparser::muparser
)

add_subdirectory(tests)