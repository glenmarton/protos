import unittest
import sys

from protos.protos import convert_to_prototype
from protos.protos import is_function_declaration
from protos.protos import is_complete
from protos.protos import process_input

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

    expect='int process(int a, int b, int c, int d, int e, int f) {'
    line ='int process(int a, int b, int c, int d, int e, int f) {'
    actual='unset'
    actual=process_input(actual, line)
    self.assertEqual(expect, actual)

  def test_read_input_multiline(self):
    expect='int doit(int a, int b,\n    int c, int d,\n    int e, int f) {\n'
    actual='unset'
    lines = [ 'int doit(int a, int b,\n', '    int c, int d,\n', '    int e, int f) {\n' ]
    for line in lines:
      actual = process_input(actual, line)

    self.assertEqual(expect, actual)
