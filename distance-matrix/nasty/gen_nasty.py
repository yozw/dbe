"""Generates the "nasty hypergraph".

Generates the hyperedges and non-hyperedges of the hypergraph F_0 in
Beaudou, Bondy, Chen, Chiniforooshan, Chudnovsky, Chvatal, Fraiman, Zwols,
"Lines in hypergraphs" Combinatorica 33.6 (2013): 633-654.
https://arxiv.org/abs/1112.0376
"""

import itertools

ROWS = ["r", "s", "t"]
COLS = ["x", "y", "z"]

# Vertex set.
V1 = {"H", "V"}
V2 = set()
for r in ROWS:
  for c in COLS:
    V2.add("{}{}".format(r, c))
V = V1 | V2

# Hyperedge set.
H = set(frozenset(edge) for edge in itertools.combinations(V2, 3))

for r in ROWS:
  R = ["{}{}".format(r, c) for c in COLS] + ["H"]
  for edge in itertools.combinations(R, 3):
    H.add(frozenset(edge))

for c in COLS:
  C = ["{}{}".format(r, c) for r in ROWS] + ["V"]
  for edge in itertools.combinations(C, 3):
    H.add(frozenset(edge))

all_triples = set(frozenset(edge) for edge in itertools.combinations(V, 3))
NH = all_triples.difference(H)

# Sort vertices and edges.
V = sorted(V)
H = sorted(H, key=lambda e: sorted(e))
NH = sorted(NH, key=lambda e: sorted(e))

# Generate output.
print "# |V| = {}".format(len(V))
print "# |H| = {}".format(len(H))
print "# |NH| = {}".format(len(NH))

print "set V := {};".format(" ".join(V))

print "set H :="
for i, edge in enumerate(H):
  print " ({}, {}, {})".format(*sorted(edge)), 
  if i % 9 == 8:
    print
print ";"

print "set NH :="
for i, edge in enumerate(NH):
  print " ({}, {}, {})".format(*sorted(edge)), 
  if i % 9 == 8:
    print
print ";"

