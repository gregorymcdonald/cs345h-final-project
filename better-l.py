#!/lusr/bin/python3
import sys, os
import argparse
import subprocess
sys.path.append('combinator')
import l_program, dag

parser = argparse.ArgumentParser()
parser.add_argument('prog', metavar='PROG', type=str,
        help='name of the L program to evaluate')
parser.add_argument('l_dir', metavar='L_DIR', type=str,
        help='directory containing all of your L source code')
parser.add_argument('so_dir', metavar='NATIVE', type=str,
        help='directory containing the compiled native code')
parser.add_argument('-r', '--run', action='store_true',
        help='flag to both combinate and run your program; omit to just combinate')

args = parser.parse_args()
lp = l_program.LProgram(args.l_dir, args.prog)

try:
	lp.combinate()
except l_program.InvalidLFileError as e:
    print('invalid program structure, all imports must be at top')
    print('\t' + e.l_file)
    sys.exit(0)
except FileNotFoundError as e:
    print('unable to find imported module')
    print('\t' + e.filename)
    sys.exit(0)
except dag.CycleError as e:
    print('circular dependency detected between these two files:')
    print('\t' + e.n1)
    print('\t' + e.n2)
    sys.exit(0)

interpreter_dir = os.getcwd()+'/interpreter'
exec_dir = ''

if args.run:
    subprocess.call(('cp '+lp.combinated_path()+' '+args.so_dir).split())
    os.chdir(args.so_dir)
    subprocess.call((interpreter_dir+'/l-interpreter '+lp.combinated_name()).split())
else:
    subprocess.call(('cp '+lp.combinated_path()+' .').split())
