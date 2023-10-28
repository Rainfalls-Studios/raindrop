file(GLOB SOURCES *.cpp)
add_library(Internal STATIC ${SOURCES})

set_target_properties(Internal PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Graphics::Internal ALIAS Internal)