find_program(glslc_executable NAMES glslc HINT Vulkan::glslc)

file(GLOB shaders *.glsl)
file(GLOB_RECURSE spv_shaders "*.spv")

foreach(source IN LISTS shaders)
	get_filename_component(FILENAME ${source} NAME)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}.spv
		COMMAND ${glslc_executable} -o ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}.spv ${source}
		DEPENDS ${source} ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Compiling ${FILENAME}"
	)
	list(APPEND SPV_shaders ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}.spv)
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_shaders})