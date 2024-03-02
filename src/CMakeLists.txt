file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

function(add_library_target TARGET_TYPE)
	message(STATUS "Building Raindrop ${TARGET_TYPE} library")

	set(TARGET Raindrop-${TARGET_TYPE})
	add_library(${TARGET} ${TARGET_TYPE} ${SOURCE})
	add_library(Raindrop::${TARGET_TYPE} ALIAS Raindrop-${TARGET_TYPE})

	set_target_properties(
		${TARGET} PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION 1
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
	)
	
	target_include_directories(
		${TARGET} PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
	)
	
	target_link_libraries(
		${TARGET}
		PRIVATE 
		spdlog::spdlog
	)

	if (DEFINED PRECOMPILED_HEADERS)
		target_precompile_headers(
			${TARGET}
			PRIVATE
			${PRECOMPILED_HEADERS}
		)
	endif()
endfunction()

if (RAINDROP_BUILD_SHARED)
	add_library_target(SHARED)
endif()

if (RAINDROP_BUILD_STATIC)
	add_library_target(STATIC)
endif()