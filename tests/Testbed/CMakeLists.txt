file(GLOB_RECURSE SOURCES *.cpp)

configure_file(config.h.in config.h)
add_executable(Raindrop-Testbed ${SOURCES})

if (RAINDROP_BUILD_SHARED)
	set(RAINDROP_LIB Raindrop-SHARED)
else()
	set(RAINDROP_LIB Raindrop-STATIC)
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

target_include_directories(
	Raindrop-Testbed

	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}
)