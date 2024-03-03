file(GLOB_RECURSE SOURCES *.cpp)

add_executable(Raindrop-Testbed ${SOURCES})

if (RAINDROP_BUILD_SHARED)
	set(RAINDROP_LIB Raindrop-STATIC)
else()
	set(RAINDROP_LIB Raindrop-SHARED)
endif()

target_link_libraries(
	Raindrop-Testbed
	${RAINDROP_LIB}
)

set_target_properties(
	Raindrop-Testbed
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY
	"${PROJECT_BINARY_DIR}/bin"
)