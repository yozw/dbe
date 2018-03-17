# De Bruijn-Erdős computational combinatorics

## Installation and compilation

* Install Bazel.
* Install Nauty.
* Clone this repo: `git clone https://github.com/yozw/dbe.git`.
* Run `bazel build -c opt ...`.
* To run unit tests: `bazel test -c opt ...`.

## Usage examples

Generate all bipartite graphs of order 5 that have no universal line:

```
nauty/geng -b 5 | bazel-out/dbe -u | nauty/showg -A
```

Generate all biconnected bipartite graphs of order 5, add one vertex in any way, remove
isomorphic duplicates, and find all graphs among the resulting graphs that have no
universal line:

```
nauty/geng -b 6 | bazel-out/add_vertex | nauty/shortg | bazel-out/dbe -u | nauty/showg -A
```

There are 172 such graphs.
To impose in addition that the starting bipartite graphs are biconnected:

```
nauty/geng -b -C 6 | bazel-out/add_vertex | nauty/shortg | bazel-out/dbe -u | nauty/showg -A
```

There are 69 such graphs. To output graphs that do not have n distinct lines
instead:

```
nauty/geng -b -C 6 | bazel-out/add_vertex | nauty/shortg | bazel-out/dbe -n | nauty/showg -A
```

Parallellizing using GNU Parallel:

```
nauty/geng -b -C 11 | parallel --block 10K --pipe 'bazel-out/add_vertex | nauty/shortg' | nauty/shortg \
    | parallel --pipe bazel-out/dbe -n | nauty/showg -A > output.txt
nauty/geng -b -d2 11 | parallel --block 5K --pipe 'bazel-out/add_vertex | nauty/shortg' | nauty/shortg \
    | parallel --block 5M --pipe bazel-out/dbe -n | nauty/showg -A > output.txt
```

These pipelines do the following:

* Generate a set of base graphs (all 2-connected bipartite graphs of order 11, and all bipartite graphs of order 11 with minimum degree, respectively).
* Partition that set, and for each partition do the following in parallel: take each graph and add a vertex in every possible way; then, remove isomorphic duplicates.
* Take the output from the previous stage and remove isomorphic duplicates.
* From the previous stage, find all graphs that do not have at least n (=12 in this case) distinct lines.
* Present the results in a human-readable form and write them to `output.txt`.

The output turns out to be empty.
