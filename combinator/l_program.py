import re
import os

import dag

class LProgram:
    
    import_regex = '((?:import\s+)(?P<module>\w+)(?:\s+in\s+))'
    import_portion_regex = '(\s*' + import_regex + ')+'
    string_lit_regex = '(".*?")'

    def __init__(self, lib_dir, prog):
        self.lib_dir = lib_dir
        self.prog = prog
        self.src_dir = os.path.dirname(prog)
        self.src_file = os.path.basename(prog)
        self.dag = dag.DAG(prog)

    def path_to(self, module):
        return self.lib_dir + '/' + module + '.Lm'

    def get_raw_code(self, l_file):
        full_prog = ''
        with open(l_file, 'r') as prog:
            for line in prog:
                full_prog += line
        if re.match(LProgram.import_portion_regex, full_prog):
            return re.sub(LProgram.import_portion_regex, '', full_prog, 1)
        else:
            return full_prog

    def valid_file(self, l_file):
        full_prog = ''
        with open(l_file, 'r') as prog:
            for line in prog:
                full_prog += line
        importless_portion = re.sub(LProgram.import_portion_regex, '', full_prog, 1)
        stringless_remainder = re.sub(LProgram.string_lit_regex, '', importless_portion)
        return re.search(LProgram.import_regex, stringless_remainder) is None

    def get_imports(self, l_file):
        full_prog = ''
        with open(l_file, 'r') as prog:
            for line in prog:
                full_prog += line
        stringless_prog = re.sub(LProgram.string_lit_regex, '', full_prog)

        required_L_files = set()
        for f in re.finditer(LProgram.import_regex, stringless_prog):
            required_L_files.add(self.path_to(f.group('module')))
        
        return required_L_files
    
    def expand_node(self, visited, l_file):
        if not self.valid_file(l_file):
            raise InvalidLFileError(l_file)
        modules = self.get_imports(l_file)
        for module in modules:
            self.dag.add(l_file, module)
        visited.add(l_file)
        for module in (modules - visited):
            self.expand_node(visited, module)

    def build_import_graph(self):
        self.expand_node(set(), self.prog)

    def combinate(self):
        try:
            self.build_import_graph()
        except InvalidLFileError as e:
            print('invalid program structure, all imports must be at top')
            print('\t' + e.l_file)
            return
        except FileNotFoundError as e:
            print('unable to find imported module')
            print('\t' + e.filename)
            return
        try:
            topo_order = self.dag.get_topological_order()
        except dag.CycleError as e:
            print('circular dependency detected between these two files:')
            print('\t' + e.n1)
            print('\t' + e.n2)
            return
        with open(self.src_dir+'/.'+self.src_file, 'w') as full_l:
            for module in topo_order:
                full_l.write(self.get_raw_code(module))

class InvalidLFileError(Exception):

    def __init__(self, l_file):
        self.l_file = l_file
