import argparse
import re

def get_imports(l_file):
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
    
    return required_L_files

def build_import_graph(lib_dir, l_file):
    import_order = []
    currents = get_imports(lib_dir + '/' + l_file)
    nexts = set()
    while len(currents) > 0:
        for f in currents:
            nexts = nexts | get_imports(lib_dir + '/' + f + '.L')
        import_order.append(set.copy(currents))
        currents = set.copy(nexts)
        nexts = set()
    return import_order

def collapse_imports(imports):
    all_imports = set()
    for i in reversed(range(len(imports))):
        imports[i] = imports[i] - all_imports
        all_imports = all_imports | imports[i]
    return imports

def get_raw_code(l_file):
    full_prog = ''
    with open(l_file, 'r') as prog:
        for line in prog:
            full_prog += line

    import_regex = '(?:import\s+)(?P<file>\w+)'
    import_regex += '\s+in\s+'
    importless_prog = re.sub(import_regex, '', full_prog)

    return importless_prog

def combinate(lib_dir, l_file):
    final_prog = ''
    final_order = reversed(collapse_imports(build_import_graph(lib_dir, l_file)))
    for prog_set in final_order:
        for prog in prog_set:
            final_prog += get_raw_code(lib_dir + '/' + prog + '.L')
    return final_prog + get_raw_code(lib_dir + '/' + l_file)

parser = argparse.ArgumentParser()
parser.add_argument('lib_dir', metavar = 'LIB_DIR', type=str)
parser.add_argument('program', metavar='PROG', type=str)

args = parser.parse_args()
full_code = combinate(args.lib_dir, args.program)
print(full_code)

