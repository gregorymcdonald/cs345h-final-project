import re

import dag

class LProgram:
    
    import_regex = '(?:import\s+)(?P<module>\w+)(?:\s+in\s+)'
    string_lit_regex = '".*?"'

    def __init__(self, src_dir, prog):
        self.src_dir = src_dir
        self.prog = prog
        self.dag = dag.DAG(prog)

    def path_to(self, module):
        return self.src_dir + '/' + module + '.Lm'

    def get_raw_code(self, l_file):
        full_prog = ''
        with open(l_file, 'r') as prog:
            for line in prog:
                full_prog += line
        return re.sub(LProgram.import_regex, '', full_prog)

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
        modules = self.get_imports(l_file)
        for module in modules:
            self.dag.add(l_file, module)
        visited.add(l_file)
        for module in (modules - visited):
            self.expand_node(visited, module)

    def build_import_dag(self):
        self.expand_node(set(), self.prog)

    def combinate(self):
        self.build_import_dag()
        try:
            topo_order = self.dag.get_topological_order()
        except dag.CycleError as e:
            print('Circular dependency detected between at least these two modules:')
            print('\t' + e.n1)
            print('\t' + e.n2)
            return
        with open(self.prog+'a', 'w') as full_l:
            for module in topo_order:
                full_l.write(self.get_raw_code(module))

