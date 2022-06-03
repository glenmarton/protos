#!/usr/bin/env python3
import argparse
import sys
from datetime import datetime
from os.path import exists


#
def main():
    args = parse_args(sys.argv[1:])
    cfile = args.source_file
    hfile = change_file_extension(cfile, 'h')

    print(f'Reading from {cfile}:')
    with open(cfile, "r") as _file:
        global_protos, local_protos = read_input(_file)

    global_body = header_build(args.purpose, global_protos, hfile)
    local_body = source_build(local_protos, cfile)

    write_file(global_body, hfile)
    write_file(local_body, cfile)


##
def parse_args(args):
    ap = argparse.ArgumentParser()
    ap.add_argument('-p', '--purpose', required=False, default='fill in purpose',
                    help='What source file is used for')
    ap.add_argument('source_file', type=str, help='Source file to process')
    return ap.parse_args(args)


def read_input(file):
    declaration = ''
    global_protos = []
    local_protos = []
    for line in file:
        declaration = process_input(declaration, line)
        if is_complete(declaration):
            prototype = convert_to_prototype(declaration)
            if is_a_global(prototype):
                global_protos.append(prototype)
            else:
                local_protos.append(prototype)
            declaration = ''
    return global_protos, local_protos


def change_file_extension(filename, ext):
    i = filename.rfind('.')
    newfile = filename[0:i + 1]
    newfile += ext
    return newfile


def header_build(purpose, protos, filename):
    macro = convert_filename_to_macro(filename)
    if exists(filename):
        top_of_file = read_file_before_prototypes(filename)
    else:
        top_of_file = new_file_boilerplate(purpose, macro, filename)

    bottom_of_file = generate_list_of(protos)
    bottom_of_file += f'#endif /* {macro} */'

    return top_of_file + bottom_of_file


def source_build(protos, filename):
    contents = ''
    read = True
    with open(filename, "r") as _file:
        for line in _file:
            if line == ' * prototypes\n':
                contents += build_prototype_section(protos)
                read = False
            elif read:
                contents += line
            elif line == '\n':
                read = True

    return contents


def write_file(body, filename):
    print(f'Writing file {filename}.')
    with open(filename, 'w') as _file:
        _file.write(body)


###
def process_input(decl, line):
    if is_function_declaration(line):
        decl = line
    elif decl != '':
        decl += line
    return decl


def is_complete(line):
    return line.find(")") >= 0 and line.find(';') < 0


def convert_to_prototype(line):
    ln = line.strip('\n')
    ln = ln.replace('{', '')
    ln = ln.strip(' ')
    if ln.find(' (') < 0:
        ln = ln.replace('(', ' (')
    ln = ln.replace('( ', '(')
    ln = ln.replace(' )', ')')
    return replace_last(ln, ')', ');')


def is_a_global(prototype):
    return prototype.find("static") < 0


def convert_filename_to_macro(name):
    macro = remove_path(name)
    macro = macro.upper()
    macro = macro.replace('.', '_')
    return f'__{macro}__'


def read_file_before_prototypes(filename):
    contents = ''
    with open(filename, "r") as _file:
        for line in _file:
            if start_prototype_list(line):
                break
            else:
                contents += line
    return contents


def new_file_boilerplate(purpose, macro='__HEADER_H__', filename='header.h'):
    if purpose == '':
        purpose = 'fill in purpose'
    filename = remove_path(filename)
    now = datetime.now()
    date = now.strftime("%Y-%m-%d")
    bp = f'/* {filename}				DATE: {date}\n * PURPOSE: {purpose}\n */\n'
    bp += f'#ifndef {macro}\n#define {macro}\n\n'
    return bp


def generate_list_of(global_list):
    count = len(global_list)
    body = f'/* Global count = {count} */\n'
    for prototype in global_list:
        body += (f'{prototype}\n')
    return body


def build_prototype_section(protos):
    contents = ' * prototypes\n */\n'
    for prototype in protos:
        contents += (f'{prototype}\n')
    contents += '\n'
    return contents


####
def replace_last(string, find, replace):
    reversed = string[::-1]
    replaced = reversed.replace(find[::-1], replace[::-1], 1)
    return replaced[::-1]


def remove_path(name):
    i = name.rfind('/')
    if i >= 0:
        name = name[i+1:len(name)]
    return name


def is_function_declaration(line):
    return line[0].islower() and line.find(';') < 0


def start_prototype_list(line):
    result = line.find('/* Global count = ')
    return result >= 0


if __name__ == "__main__":
    main()
