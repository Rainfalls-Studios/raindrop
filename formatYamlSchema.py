import re
import json
import glob
import os

def parse_enums(c_file_content):
    enum_pattern = re.compile(r'typedef enum ([a-zA-Z_]+) \{([^}]+)\} ([a-zA-Z_]+);')
    enum_matches = enum_pattern.finditer(c_file_content)

    enums = {}
    for enum_match in enum_matches:
        enum_name = enum_match.group(1)
        enums[enum_name] = [value.strip().split('=')[0].strip() for value in enum_match.group(2).split(',')]

    return enums

def generate_json_schema(enum_name, enum_values):
    if not enum_name or not enum_values:
        return None

    schema = {
        enum_name: {
            "type": "string",
            "enum": enum_values
        }
    }

    return schema

def write_to_file(output_filename, schemas):
    with open(output_filename, 'w') as output_file:
        json.dump(schemas, output_file, indent=4)

def main(input_filename, output_filename):
    with open(input_filename, 'r') as input_file:
        c_file_content = input_file.read()

        enums = parse_enums(c_file_content)

        schemas = {}
        for enum_name, enum_values in enums.items():
            schema = generate_json_schema(enum_name, enum_values)
            if schema:
                schemas.update(schema)

        if schemas:
            write_to_file(output_filename, schemas)
            print(f"Schemas for enums written to {output_filename}")
        else:
            print("Error parsing the C file.")

def find_vulkan_core_header():
    print("Looking for vulkan_core.h ...")
    root_directory = "/usr/"

    header_files = glob.glob(os.path.join(root_directory, "**/vulkan_core.h"), recursive=True)

    if header_files:
        return header_files[0]
    else:
        print("Error: vulkan_core.h not found.")
        return None

if __name__ == "__main__":
    input_file_path = find_vulkan_core_header()
    output_file_path = "schem_output.json"

    main(input_file_path, output_file_path)
