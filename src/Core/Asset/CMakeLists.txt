file(GLOB SOURCES *.cpp)
add_library(Asset STATIC ${SOURCES})

set_target_properties(Asset PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Core::Asset ALIAS Asset)