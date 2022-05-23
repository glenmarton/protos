import unittest
import sys

from logscribe.eprint import eprint
from logscribe.fileio import read_file
from logscribe.fileio import read_yaml_file

class TestProtos(unittest.TestCase):

  def test_read_file(self):
    fname = './data/template.yml'
    contents = read_yaml_file(fname)
    actual = contents.keys()
    expect = "dict_keys(['issue', 'type', 'description', 'model', 'milestone'])"
    self.assertEqual(str(actual), str(expect))

  def test_read_file_without_file(self):
    fname = 'not_a_file.conf'
    with self.assertRaises(FileNotFoundError): read_file(fname)

  def test_read_conf(self):
    fname = './data/conftest.conf'
    actual = read_file(fname)
    expect = ['YAML_PATH=./yaml_dir', "CHANGELOG='test_path/test_CHANGELOG.md'", 'a']
    self.assertEqual(str(actual), str(expect))
