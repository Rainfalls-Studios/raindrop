import os
import re

def locate_vulkan_core():
    for root, dirs, files in os.walk("/"):
        if 'vulkan_core.h' in files:
            return os.path.join(root, 'vulkan_core.h')
    return None

def parse_vkformat_enum(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    vkformat_enum_pattern = re.compile(r'enum\s+VkFormat\s*{([^}]*)}', re.DOTALL)
    match = vkformat_enum_pattern.search(content)
    
    if not match:
        print("VkFormat enum not found.")
        return None

    vkformat_enum = match.group(1)
    vkformat_entries = vkformat_enum.split(',')
    
    formats = []
    for entry in vkformat_entries:
        entry = entry.strip()
        if entry and not re.match(r'\b(VK_FORMAT_MAX_ENUM|VK_FORMAT_UNDEFINED)\b', entry):
            format_match = re.match(r'(\w+)\s*=\s*(\d+)', entry)
            if format_match:
                formats.append((format_match.group(1), int(format_match.group(2))))
            else:
                format_match = re.match(r'(\w+)', entry)
                if format_match:
                    formats.append((format_match.group(1), None))
    
    return formats

def fill_missing_indices(formats):
    last_index = -1
    for i, (name, index) in enumerate(formats):
        if index is None:
            formats[i] = (name, last_index + 1)
        last_index = formats[i][1]

def generate_cpp_loops(formats):
    loops = []
    start = 0

    for i in range(1, len(formats)):
        if formats[i][1] != formats[i-1][1] + 1:
            loops.append((formats[start][0], formats[i-1][0], formats[start][1], formats[i-1][1]))
            start = i

    loops.append((formats[start][0], formats[-1][0], formats[start][1], formats[-1][1]))
    return loops

def write_cpp_file(output_folder, loops):
    # Ensure the folder exists
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    output_path = os.path.join(output_folder, "formats_loop.txt")
    with open(output_path, 'w') as file:
        file.write("// generated using scrips/generate_formats_loops.py\n")
        file.write("#define for_each_formats(fnc) ")
        for loop in loops:
            file.write(f'for (VkFormat fmt = {loop[0]}; fmt <= {loop[1]}; fmt = static_cast<VkFormat>(fmt + 1))')
            file.write('{fnc}')

    print(f"Generated C++ file at {output_path}")

def main():
    vulkan_core_path = locate_vulkan_core()
    if not vulkan_core_path:
        print("vulkan_core.h not found.")
        return

    print(f"Found vulkan_core.h at: {vulkan_core_path}")
    formats = parse_vkformat_enum(vulkan_core_path)
    if not formats:
        print("VkFormat enum not found in vulkan_core.h.")
        return

    print("Parsed VkFormat enum entries:")
    for name, index in formats:
        print(f"  {name} = {index}")

    fill_missing_indices(formats)
    loops = generate_cpp_loops(formats)
    
    output_folder = "out/"  # Define your desired output folder here
    write_cpp_file(output_folder, loops)

if __name__ == "__main__":
    main()
