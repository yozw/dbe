# The nasty hypergraph

The "nasty hypergraph" (F_0 in
Beaudou, Bondy, Chen, Chiniforooshan, Chudnovsky, Chvatal, Fraiman, Zwols,
[Lines in hypergraphs](https://arxiv.org/abs/1112.0376) Combinatorica 33.6 (2013): 633-654)
has the following vertices:

```
      V
  rx ry rz
H sx sy sz
  tx ty tz
```

and has the following hyperedges:

* all sets of three vertices other than H and V 
* all sets {H, ac, ad}
* all sets {V, ac, bc}

The code in this directory was used to find the following "distance matrix" (in quotes
because it doesn't respect the triangle inequality) corresponding to this hypergraph:

```
H  | 0 5 | 4 5 6 | 4 5 6 | 4 5 6
V  | 5 0 | 2 2 2 | 5 5 5 | 8 8 8
--------------------------------
rx | 4 2 | 0 1 2 | 3 2 1 | 6 5 4
ry | 5 2 | 1 0 1 | 4 3 2 | 7 6 5
rz | 6 2 | 2 1 0 | 5 4 3 | 8 7 6
--------------------------------
sx | 4 5 | 3 4 5 | 0 1 2 | 3 2 1
sy | 5 5 | 2 3 4 | 1 0 1 | 4 3 2
sz | 6 5 | 1 2 3 | 2 1 0 | 5 4 3
--------------------------------
tx | 4 8 | 6 7 8 | 3 4 5 | 0 1 2
ty | 5 8 | 5 6 7 | 2 3 4 | 1 0 1
tz | 6 8 | 4 5 6 | 1 2 3 | 2 1 0
```

This "metric space" corresponding to this matrix has 10 lines and no mighty pair.
