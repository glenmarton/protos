#!/usr/bin/env python3
import sys
from protos.eprint import eprint

#
#   f u n c t i o n s
#
def convert_to_prototype(line):
    ln = line.strip('\n')
    ln = ln.replace('{', '')
    ln = ln.strip(' ')
    if ln.find(' (') < 0:
        ln = ln.replace('(', ' (')
    ln = ln.replace('( ', '(')
    ln = ln.replace(' )', ')')
    return ln.replace(')', ');')

def read_input(file):
    declaration = ''
    for line in file:
        declaration = process_input(declaration, line)
        if is_complete(declaration):
          prototype = convert_to_prototype(declaration)
          if prototype.find("static") >= 0:
              print(f'local : {prototype}')
          else:
              print(f'public : {prototype}')
          declaration=''


def process_input(decl, line):
    if is_function_declaration(line):
        decl = line
    elif decl != '':
        print (f'Building {decl}.')
        decl += line
    return decl

def is_function_declaration(line):
    return line[0].islower() and line.find(';') < 0

def is_complete(line):
    return line.find(")") >= 0 and line.find(';') < 0

def record(prototype):
    if prototype.find("static") >= 0:
        print(f'local : {prototype}')
    else:
        print(f'public : {prototype}')

def main():
    if len(sys.argv) > 1:
        with open(sys.argv[1], "r") as _file:
          read_input(_file)
    else:
        read_input(sys.stdin)

if __name__ == "__main__":
    main()
