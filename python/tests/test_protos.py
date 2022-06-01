import unittest
import sys

from datetime import datetime
from protos.protos import convert_to_prototype
from protos.protos import is_function_declaration
from protos.protos import is_complete
from protos.protos import process_input
from protos.protos import change_file_extension
from protos.protos import generate_list_of
from protos.protos import new_file_boilerplate

class TestProtos(unittest.TestCase):

    def test_prototype_conversion(self):
        expect = 'int doit (int a, int b, int c);'

        actual = convert_to_prototype('int doit(int a, int b, int c)')
        self.assertEqual(expect, actual, 'Prototype is not correct for single line.')

    def test_prototype_conversion_spaces_in_parenthesis(self):
        expect = 'int doit (int a, int b, int c);'
        actual = convert_to_prototype('int doit( int a, int b, int c )')
        self.assertEqual(expect, actual, 'Prototype is not correct for single line.')

    def test_prototype_conversion_brace_with_cpp_comment(self):
        expect = 'int doit (int a, int b, int c);  // comment'
        actual = convert_to_prototype('int doit (int a, int b, int c) { // comment')
        self.assertEqual(expect, actual, 'Prototype is not correct for single line.')

    def test_prototype_conversion_with_c_comment(self):
        expect = 'int doit (int a, int b, int c); /* comment */'
        actual = convert_to_prototype('int doit (int a, int b, int c) /* comment */')
        self.assertEqual(expect, actual, 'Prototype is not correct for single line.')

    def test_function_declaration_valid(self):
        actual = is_function_declaration('int add(int a, int b)')
        self.assertTrue(actual)

    def test_function_declaration_commented(self):
        actual = is_function_declaration('//int add(int x, int y)')
        self.assertFalse(actual)

    def test_function_declaration_Ccommented(self):
        actual = is_function_declaration('/* int add(int x, int y) */')
        self.assertFalse(actual)

    def test_function_declaration_invalid_return1(self):
        actual = is_function_declaration('2_ints add(int x, int y)')
        self.assertFalse(actual)

    def test_function_declaration_invalid_return2(self):
        actual = is_function_declaration('Int add(int x, int y)')
        self.assertFalse(actual)

    def test_function_declaration_actually_a_function_call(self):
        actual = is_function_declaration('    add(x, y);')
        self.assertFalse(actual)

    def test_function_declaration_actually_a_function_call_not_indented(self):
        actual = is_function_declaration('add(x, y);')
        self.assertFalse(actual)

    def test_close_of_single_line_function_declaration(self):
        actual = is_complete('void doit(int a)');
        self.assertTrue(actual)

    def test_close_of_function_declaration(self):
        actual = is_complete('void doit(int a');
        self.assertFalse(actual)

    def test_close_of_function_declaration_multiline_start(self):
        actual = is_complete('void doit(int a');
        self.assertFalse(actual)

    def test_close_of_function_declaration_multiline_end(self):
        actual = is_complete('	int a)');
        self.assertTrue(actual)

    def test_close_of_function_declaration_actually_a_call(self):
        actual = is_complete('	doit (a);');
        self.assertFalse(actual)

    def test_read_input_singleline(self):
        actual='unset'
        line ='int process(int a, int b, int c, int d, int e, int f) {'
        actual=process_input(actual, line)

        expect='int process(int a, int b, int c, int d, int e, int f) {'
        self.assertEqual(expect, actual)

    def test_read_input_multiline(self):
        expect='int doit(int a, int b,\n    int c, int d,\n    int e, int f) {\n'
        actual='unset'
        lines = [ 'int doit(int a, int b,\n', '    int c, int d,\n', '    int e, int f) {\n' ]
        for line in lines:
            actual = process_input(actual, line)

        self.assertEqual(expect, actual)

    def test_change_extension(self):
        expect = 'report.md'
        actual = change_file_extension('report.txt', 'md')
        self.assertEqual(expect, actual)

    def test_change_extension_no_ext(self):
        expect = 'filename.'
        actual = change_file_extension('filename.txt', '')
        self.assertEqual(expect, actual)

    def test_change_extension_no_filename(self):
        expect = 'txt'
        actual = change_file_extension('', 'txt')
        self.assertEqual(expect, actual)

    def test_boilerplate_no_purpose_nor_filename(self):
        now = datetime.now()
        date = now.strftime("%Y-%m-%d")
        expect = f'/* header.h				DATE: {date}\n * PURPOSE: Header file for header.h.\n */\n#ifndef __HEADER_H__\n#define __HEADER_H__\n\n'
        actual = new_file_boilerplate('')
        self.assertEqual(expect, actual)

    def test_generate_list_of_no_protos(self):
        global_list = []
        expect = '/* Global count = 0 */\n'
        actual = generate_list_of(global_list)
        self.assertEqual(expect, actual)

    def test_generate_list_of_with_protos(self):
        global_list = ['int add(int a, int b);', 'int sub(int a, int b);']
        expect = '/* Global count = 2 */\nint add(int a, int b);\nint sub(int a, int b);\n'
        actual = generate_list_of(global_list)
        self.assertEqual(expect, actual)
