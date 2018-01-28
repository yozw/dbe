# De Bruijn-Erdos analyser

Installation and compilation:

```
git clone https://github.com/yozw/dbe.git
cd dbe
wget http://users.cecs.anu.edu.au/~bdm/nauty/nauty26r11.tar.gz
tar xvzf nauty26r11.tar.gz
mv nauty26r11 nauty
cd nauty
./configure
make
cd ..
./make
```

Generate all bipartite graphs of order 5 that have no universal line:

```
nauty/geng -b 5 | ./dbe -u | nauty/showg -A
```

Generate all biconnected bipartite graphs of order 5, add one vertex in any way, remove
isomorphic duplicates, and find all graphs among the resulting graphs that have no
universal line:

```
nauty/geng -b 5 | ./add_vertex | nauty/shortg | ./dbe -u | nauty/showg -A
```

(There are 43 such graphs).
To impose in addition that the starting bipartite graphs are biconnected:

```
nauty/geng -b -C 5 | ./add_vertex | nauty/shortg | ./dbe -u | nauty/showg -A
```

To are no such graphs.
