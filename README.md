# De Bruijn-Erdos analyser

Set up:

```
NAUTY=../nauty26r11
```

Generate all bipartite graphs of order 5 that have no universal line:

```
$NAUTY/geng -b 5 | ./dbe -u | $NAUTY/showg -A
```

Generate all biconnected bipartite graphs of order 5, add one vertex in any way, remove
isomorphic duplicates, and find all graphs among the resulting graphs that have no
universal line:

```
$NAUTY/geng -b 5 | ./add_vertex | $NAUTY/shortg | ./dbe -u | $NAUTY/showg -A
```

(There are 43 such graphs).
To impose in addition that the starting bipartite graphs are biconnected:

```
$NAUTY/geng -b -C 5 | ./add_vertex | $NAUTY/shortg | ./dbe -u | $NAUTY/showg -A
```

To are no such graphs.
