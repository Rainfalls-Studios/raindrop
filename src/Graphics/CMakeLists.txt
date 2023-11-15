file(GLOB SOURCES *.cpp)
add_library(Graphics STATIC ${SOURCES})

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory("Internal")
add_subdirectory(Buffers)
add_subdirectory(Utils)
add_subdirectory(Shaders)

target_include_directories(Graphics PRIVATE ${Vulkan_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

target_link_libraries(Graphics
	Raindrop::Graphics::Internal
	Raindrop::Graphics::Buffers
	Raindrop::Graphics::Utils
	Raindrop::Graphics::Shaders
	Vulkan::Vulkan
	SDL2::SDL2
)

set_target_properties(Graphics PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Graphics ALIAS Graphics)