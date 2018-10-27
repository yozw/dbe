"""Generates the line graph of K_n."""

import sys

def main(n):
  edges = set()
  vmap = [frozenset([i, j]) for i in range(n) for j in range(i+1, n)]
  m = len(vmap)
  for u in range(m):
    for v in range(u+1,len(vmap)):
      if vmap[u].intersection(vmap[v]):
        edges.add(frozenset([u, v]))

  print "n={} m".format(m)
  for u in range(m):
    line = []
    for v in range(m):
      if frozenset([u, v]) in edges:
        line.append(1)
      else:
        line.append(0)
    print " ".join(str(x) for x in line)


if __name__ == '__main__':
  main(int(sys.argv[1]))
