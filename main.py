import os
import re

def extract_classes(file_content):
    # Use regular expression to find class declarations
    class_pattern = re.compile(r'class (\w+)')
    return class_pattern.findall(file_content)

def create_header_file(class_name):
    return f"{class_name}.h"

def create_source_file(class_name):
    return f"{class_name}.cpp"

def write_to_file(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)

def main():
    # Read content from main.cpp
    with open('main.cpp', 'r') as main_file:
        main_content = main_file.read()

    # Create src folder if it doesn't exist
    os.makedirs('src', exist_ok=True)

    # Extract class names
    class_names = extract_classes(main_content)

    # Create header and source files for each class in src folder
    for class_name in class_names:
        header_content = f"#ifndef {class_name.upper()}_H\n#define {class_name.upper()}_H\n\nclass {class_name} {{\npublic:\n    {class_name}();\n    ~{class_name}();\n\nprivate:\n    // Add your class members here\n}};\n\n#endif // {class_name.upper()}_H"
        source_content = f"#include \"{class_name}.h\"\n\n{class_name}::{class_name}() {{\n    // Constructor implementation\n}}\n\n{class_name}::~{class_name}() {{\n    // Destructor implementation\n}}\n\n// Add other member function implementations here"

        header_path = os.path.join('src', create_header_file(class_name))
        source_path = os.path.join('src', create_source_file(class_name))

        write_to_file(header_path, header_content)
        write_to_file(source_path, source_content)

main()
