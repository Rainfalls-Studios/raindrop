set(PCH_TARGET "${CMAKE_CURRENT_BINARY_DIR}/precompiled_common.pch")
add_library(pch_target INTERFACE)
target_precompile_headers(pch_target INTERFACE ${PCH_PATH})

# === ENGINE ===

option(RAINDROP_EDITOR "compile the engine into editor mode, which mean with editor menu and debug features" ON)

if (${RAINDROP_EDITOR})
	add_definitions(-DRAINDROP_EDITOR -DRAINDROP_EDITOR_PATH="${EDITOR_PATH}")
endif()

file(GLOB_RECURSE SOURCES *.cpp)
add_library(RaindropEngine STATIC ${SOURCES})

target_link_libraries(RaindropEngine PRIVATE ${Vulkan_LIBRARIES} SDL2::SDL2-static tinyobjloader::tinyobjloader)
target_link_libraries(RaindropEngine PUBLIC easyloggingpp tinyxml2 imgui pch_target)

set_target_properties(RaindropEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)