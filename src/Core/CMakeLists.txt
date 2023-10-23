file(GLOB SOURCES *.cpp)
add_library(Core STATIC ${SOURCES})


add_subdirectory(Asset)
add_subdirectory(Event)
add_subdirectory(Registry)
add_subdirectory(Scene)


target_link_libraries(Core
	Raindrop::Core::Asset
	Raindrop::Core::Event
	Raindrop::Core::Registry
	Raindrop::Core::Scene
)

set_target_properties(Core PROPERTIES
	C_VISIBILITY_PRESET "hidden"
	CXX_VISIBILITY_PRESET "hidden"
	VISIBILITY_INLINES_HIDDEN 1
)

add_library(Raindrop::Core ALIAS Core)