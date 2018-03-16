import subprocess
import unittest


def RunAddVertex(input, args=()):
  process = subprocess.Popen(['add_vertex'] + list(args),
      stdin=subprocess.PIPE,
      stderr=subprocess.PIPE,
      stdout=subprocess.PIPE)
  stdout, stderr = process.communicate(input=input)
  stdout = [line for line in stdout.split('\n') if line]
  stderr = [line for line in stderr.split('\n') if line]
  return stdout, stderr


class AddVertexTest(unittest.TestCase):

  C5='DUW'  # The 5-cycle.

  def testAllVertexAdditions(self):
    stdout, stderr = RunAddVertex(self.C5)
    self.assertIn(':EgGEQ_^', stdout)
    self.assertIn(':EgGEQ_n', stdout)
    self.assertIn(':EgGEQcs', stdout)  # C5 with adjacent clone
    self.assertIn(':EgGEQg~', stdout)  # C5 with non-adjacent clone
    self.assertEqual(len(set(stdout)), 26)

  def testClones(self):
    stdout, stderr = RunAddVertex(self.C5, ['-t=2'])
    self.assertIn(':EgGEQcs', stdout)  # C5 with adjacent clone
    self.assertIn(':EgGEQg~', stdout)  # C5 with non-adjacent clone
    self.assertEqual(len(set(stdout)), 10)

  def testAdjacentClones(self):
    stdout, stderr = RunAddVertex(self.C5, ['-t=3'])
    self.assertIn(':EgGEQcs', stdout)  # C5 with adjacent clone
    self.assertEqual(len(set(stdout)), 5)

  def testNonAdjacentClones(self):
    stdout, stderr = RunAddVertex(self.C5, ['-t=4'])
    self.assertIn(':EgGEQg~', stdout)  # C5 with non-adjacent clone
    self.assertEqual(len(set(stdout)), 5)


if __name__ == '__main__':
    unittest.main()