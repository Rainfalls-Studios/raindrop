file(GLOB SOURCES *.cpp)
add_library(Utils STATIC ${SOURCES})

set_target_properties(Utils PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Graphics::Utils ALIAS Utils)