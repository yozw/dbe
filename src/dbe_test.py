import subprocess
import unittest

ALL_CONNECTED_GRAPHS_ORDER_5 = [
  'D?{', 'DCw', 'DC{', 'DEw', 'DEk', 'DE{', 'DFw', 'DF{', 'DQo', 'DQw', 'DQ{',
  'DUW', 'DUw', 'DU{', 'DTw', 'DT{', 'DV{', 'D]w', 'D]{', 'D^{', 'D~{']


def RunDbe(input, args=()):
  if not isinstance(args, (tuple, list)):
    args = [args]
  if isinstance(input, (tuple, list)):
    input = '\n'.join(input)

  process = subprocess.Popen(['dbe'] + list(args),
      stdin=subprocess.PIPE,
      stderr=subprocess.PIPE,
      stdout=subprocess.PIPE)
  stdout, stderr = process.communicate(input=input)
  stdout = [line for line in stdout.split('\n') if line]
  stderr = [line for line in stderr.split('\n') if line]
  return stdout, stderr


class DbeTest(unittest.TestCase):

  C5='DUW'  # The 5-cycle.

  def testPentagon(self):
    stdout, stderr = RunDbe(self.C5, '-o=1')
    self.assertEqual(stdout, ['10,0,5'])  # Number of lines, universal pairs, AMRZ gap.

  def testNonUniversal(self):
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-u')
    self.assertEqual(stdout, [':Dk@I@G~', ':DgH_QN', ':DgGEQ', ':DgGE@G~', ':DgGCgCb', ':Da@_Q_QN'])

  def testFewerThanNLines(self):
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-n')
    self.assertEqual(len(set(stdout)), 10)

  def testNmax(self):
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-nmax=5')
    self.assertEqual(len(set(stdout)), 10)
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-nmax=3')
    self.assertEqual(stdout, [':DgH_^'])  # Path of length 4
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-nmax=1')
    self.assertEqual(stdout, [':DgH_^'])  # Path of length 4

  def testNmin(self):
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-nmin=5')
    self.assertEqual(len(set(stdout)), 11)
    stdout, stderr = RunDbe(ALL_CONNECTED_GRAPHS_ORDER_5, '-nmin=0')
    self.assertEqual(len(set(stdout)), len(ALL_CONNECTED_GRAPHS_ORDER_5))


if __name__ == '__main__':
    unittest.main()