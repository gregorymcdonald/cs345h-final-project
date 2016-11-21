import argparse
import re

def get_imports_and_raw_code(l_file):
    full_prog = ''
    with open(l_file, 'r') as prog:
        for line in prog:
            full_prog += line

    string_lit_regex = '".*?"'
    stringless_prog = re.sub(string_lit_regex, '', full_prog)

    import_regex = '(?:import\s+)(?P<file>\w+)'
    required_L_files = set()
    for f in re.finditer(import_regex, stringless_prog):
        required_L_files.add(f.group('file'))

    import_regex += '\s+in\s+'
    importless_prog = re.sub(import_regex, '', full_prog)

    return required_L_files, importless_prog

def combinate(l_file):
    full_import_code = ''
    next_imports, raw_code = get_imports_and_raw_code(l_file)
    imports = set()
    while len(next_imports) > 0:
        new_imports = set()
        for import_file in next_imports:
            import_file_imports, import_file_code = get_imports_and_raw_code(import_file + '.L')
            full_import_code += import_file_code + '\n'
            new_imports = new_imports | import_file_imports
        imports = imports | next_imports
        next_imports = new_imports - imports
    return imports, full_import_code + raw_code

parser = argparse.ArgumentParser()
parser.add_argument('program', metavar='PROG', type=str)

args = parser.parse_args()
imports, full_import_code = combinate(args.program)
print(imports)
print(full_import_code)
