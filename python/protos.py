#!/usr/bin/env python3
import sys

#
#   f u n c t i o n s
#
def prepare_end_of_line(line):
    ln = line.strip('\n')
    ln = ln.strip('{')
    ln = ln.strip(' ')

    return ln.replace(')', ');')

def read_input(file):
    mlproto = ''
    for line in file:
        if is_function_declaration(line):
            if close_of_function_declaration_on_this(line):
                prototype = prepare_end_of_line(line)
                record(prototype)
            else:
                print (f'Found a multiline prototype {mlproto}.')
                mlproto = line
        elif mlproto != '':
            print (f'Building a multiline prototype {mlproto}.')
            if close_of_function_declaration_on_this(line):
                mlproto += line
                record(mlproto)
                mlproto = ''
            else:
                mlproto += prototype

def is_function_declaration(line):
    return line[0].isalpha() and line[-2] != ';'

def close_of_function_declaration_on_this(line):
    return line.find(")") >= 0;

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
