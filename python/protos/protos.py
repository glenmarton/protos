#!/usr/bin/env python3
import sys

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


def process_input(multi, line):
    if is_function_declaration(line):
        multi = line
    elif is_complete(multi) == False:
        print (f'Building {multi}.')
        multi += line
    return multi

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
