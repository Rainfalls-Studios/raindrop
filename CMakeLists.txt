cmake_minimum_required(VERSION 3.30)
project(Raindrop)

# set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(SPDLOG_BUILD_STATIC ON)
set(SDL_STATIC ON)

# dependencies
add_subdirectory(external)
find_package(Vulkan REQUIRED)

# On Windows, use static runtime libraries
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(Raindrop STATIC ${SOURCES})

target_compile_features(Raindrop PRIVATE cxx_std_20)

target_include_directories(Raindrop PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries(
	Raindrop

	PRIVATE
    -static-libgcc
    -static-libstdc++
    SDL3::SDL3-static
    vk-bootstrap
	glm::glm
	Vulkan::Vulkan
    spdlog::spdlog_header_only
    EnTT::EnTT
	
)

# For Windows-specific static linking
if(WIN32)
    target_link_libraries(Raindrop PRIVATE
        -static
        -lpthread
        -lwinpthread
    )
endif()

add_subdirectory(tests)