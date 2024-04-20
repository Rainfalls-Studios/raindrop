file(GLOB_RECURSE SOURCES *.cpp)

configure_file(config.h.in config.h)
add_library(barebone SHARED ${SOURCES})

if (RAINDROP_BUILD_SHARED)
	set(RAINDROP_LIB Raindrop-SHARED)
else()
	set(RAINDROP_LIB Raindrop-STATIC)
endif()

target_link_libraries(
	barebone

	${RAINDROP_LIB}
	spdlog::spdlog_header_only
)

set_target_properties(
	barebone
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/modules"
	DEBUG_POSTFIX ""
	PREFIX ""
)

target_include_directories(
	barebone
	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}
)