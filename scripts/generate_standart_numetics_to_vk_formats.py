# Define the output folder and file path
output_folder = 'out/'
output_file = output_folder + 'type_to_format.hpp'

# Ensure the output directory exists
import os
os.makedirs(output_folder, exist_ok=True)

# Define the types and their corresponding Vulkan formats
type_to_vk_format = {
	'int8_t': ('8', 'SINT'),
	'uint8_t': ('8', 'UINT'),
	'int16_t': ('16', 'SINT'),
	'uint16_t': ('16', 'UINT'),
	'int32_t': ('32', 'SINT'),
	'uint32_t': ('32', 'UINT'),
	'int64_t': ('64', 'SINT'),
	'uint64_t': ('64', 'UINT'),
	'float': ('32', 'SFLOAT'),
	'double': ('64', 'SFLOAT')
}

# Generate the combinations for glm::vec<type, n>
vector_sizes = [2, 3, 4]

# Start writing the C++ file
with open(output_file, "w") as file:
	file.write("#ifndef __RAINDROP_GRAPHICS_UTILS_STD_NUMERICS_TO_VK_FORMAT_HPP__\n")
	file.write("#define __RAINDROP_GRAPHICS_UTILS_STD_NUMERICS_TO_VK_FORMAT_HPP__\n")
	file.write("#include <vulkan/vulkan.h>\n")
	file.write("#include <glm/glm.hpp>\n\n")

	file.write("// Auto-generated template specializations for typeToFormat<T>()\n\n")

	file.write("namespace Raindrop::Graphics::Utils{\n")
	file.write("\ttemplate<typename T>\n")
	file.write("\tVkFormat typeToFormat() noexcept;\n\n")
	for base_type, vk_format in type_to_vk_format.items():
		# Base type template specialization
		file.write(f"\ttemplate<>\n")
		file.write(f"\tVkFormat typeToFormat<{base_type}>() noexcept{{\n")
		file.write(f"\t\treturn VK_FORMAT_R{vk_format[0]}_{vk_format[1]};\n")
		file.write(f"\t}}\n\n")

		# Vector types specializations
		for size in vector_sizes:
			file.write(f"\ttemplate<>\n")
			file.write(f"\tVkFormat typeToFormat<glm::vec<{size}, {base_type}>>() noexcept{{\n")

			f = 'R' + vk_format[0]

			if (size >= 2):
				f += 'G' + vk_format[0]
			
			if (size >= 3):
				f += 'B' + vk_format[0]

			if (size >= 4):
				f += 'A' + vk_format[0]

			file.write(f"\t\treturn VK_FORMAT_{f}_{vk_format[1]};\n")
			file.write(f"\t}}\n\n")
	
	file.write("}\n")
	file.write("#endif")
