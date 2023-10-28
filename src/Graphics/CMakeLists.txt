file(GLOB SOURCES *.cpp)
add_library(Graphics STATIC ${SOURCES})

add_subdirectory("Internal")
add_subdirectory(Buffers)
add_subdirectory(Utils)

target_link_libraries(Graphics
	Raindrop::Graphics::Internal
	Raindrop::Graphics::Buffers
	Raindrop::Graphics::Utils
)

set_target_properties(Graphics PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Graphics ALIAS Graphics)