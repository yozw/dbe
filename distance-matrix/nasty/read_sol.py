"""Read a CPLEX solution for the "nasty hypergraph" and prints out
the corresponding distance matrix."""

import sys

V = set()
D = dict()
PRINT_MATRIX = False

for line in sys.stdin:
  line = line.strip()
  if not line.startswith('<variable name="d('):
    continue
  tokens = line.split('"')
  if not tokens[1].startswith("d("):
    raise ValueError("Unexpected input")
  x, y = tokens[1].replace("d(", "").replace(")", "").split(",")
  distance = int(round(float(tokens[5])))
  D[x, y] = distance
  D[y, x] = distance
  D[x, x] = 0
  D[y, y] = 0
  V.add(x)
  V.add(y)

V = sorted(V)

# Print input for dbe.
print len(V),
for i in range(len(V)):
  for j in range(i+1, len(V)):
    print D[V[i], V[j]],
    D[V[j], V[i]] = D[V[i], V[j]]
print "NastyHypergraph"

# Print as matrix, if requested.
if PRINT_MATRIX:
  print " ".join(V)
  for i, x in enumerate(V):
    if i % 3 == 2:
      print "-" * 32
    print "%-2s" % x, 
    for j, y in enumerate(V):
      if j == 0 or j % 3 == 2:
        print "|",
      print D[x, y],
    print
