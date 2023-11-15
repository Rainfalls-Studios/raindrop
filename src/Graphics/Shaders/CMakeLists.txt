file(GLOB SOURCES *.cpp)
add_library(Shaders STATIC ${SOURCES})

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

set_target_properties(Shaders PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

target_link_libraries(Shaders PRIVATE shaderc_combined)
target_include_directories(Shaders PRIVATE ${Vulkan_INCLUDE_DIRS})

add_library(Raindrop::Graphics::Shaders ALIAS Shaders)