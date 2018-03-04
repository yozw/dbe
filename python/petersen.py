from __future__ import print_function

import collections
import itertools


def permutations(X):
  """Generates all permutation mappings pi: X -> X."""
  X = sorted(X)
  for pi in itertools.permutations(X):
    pi = {X[i]: value for i, value in enumerate(pi)}
    yield pi


class UnorderedTuple(frozenset):
  def __new__(cls, *values):
    return super(UnorderedTuple, cls).__new__(cls, values)

  def permute(self, pi):
    values = [pi[x] for x in self]
    return UnorderedTuple(*values)

  def __repr__(self):
    return "{%s}" % ",".join(str(x) for x in self)


class DistanceMatrix(dict):
  def __getitem__(self, key):
    key = UnorderedTuple(*key)
    return super(DistanceMatrix, self).__getitem__(key)

  def __setitem__(self, key, value):
    key = UnorderedTuple(*key)
    return super(DistanceMatrix, self).__setitem__(key, value)


# Define ground set.
X = range(1, 6)

# Define vertex set.
V = set(UnorderedTuple(i, j) for i, j in itertools.combinations(X, 2))

# Define distance matrix.
d = DistanceMatrix()  # Distance in complement Petersen.
for u, v in itertools.combinations(V, 2):
  d[u, v] = 1 if u.intersection(v) else 2

# Define edge set of hypergraph H.
EH = set()
for u, v, w in itertools.combinations(V, 3):
  ds = [d[u, v], d[v, w], d[u, w]]
  if sorted(ds) == [1, 1, 2]:
    EH.add(UnorderedTuple(u, v, w))

# Define particular vertices to constrain permutation.
x = UnorderedTuple(1, 2)
xp = UnorderedTuple(1, 2)
y = UnorderedTuple(3, 4)
yp = UnorderedTuple(1, 3)

# Consider all permutations of the vertex set.
for pi in permutations(V):
  # Only consider permutations that map x to xp and y to yp.
  if pi[x] == xp and pi[y] == yp:
    permuted_EH = set(edge.permute(pi) for edge in EH)
    if permuted_EH == EH:
      print("Found an automorphism:")
      print(len([v for v, vp in pi.items() if v == vp]))
      for v, vp in sorted(pi.items()):
        print("  {} -> {}".format(v, vp))
