#include <iostream>

#include "common.h"
#include "graphs.h"

/** Reads a non-empty line from stdin. **/
boost::optional<std::string> ReadLine() {
  std::string line;
  do {
    if (std::cin.eof()) {
      return boost::none;
    }
    std::getline(std::cin, line);
  } while (line.length() == 0);
  return line;
}

/** Reads a sparse6-encoded graph from stdin. **/
boost::optional<Graph> ReadGraph() {
  boost::optional<std::string> line = ReadLine();
  if (!line) {
    return boost::none;
  }

  // Parse sparse graph encoded in line.
  sparsegraph sg;
  SG_INIT(sg);
  int nloops;
  char *repr = const_cast<char *>(line.get().c_str());
  stringtosparsegraph(repr, &sg, &nloops);
  if (nloops != 0) {
    Error("Loops are not supported");
  }

  Graph graph = SparseGraphToBgl(sg);
  SG_FREE(sg);
  return graph;
}

/** Writes a graph in sparse6 encoding to stdout. **/
void WriteGraph(const Graph &graph) {
  sparsegraph sg;
  BglToSparseGraph(graph, sg);
  char *sgraph6 = sgtos6(&sg);
  std::cout << sgraph6;
  // Note: sgraph6 should not be deallocated, because it refers to global
  // variable in gtools.c.
  SG_FREE(sg);
}

/** Converts sparsegraph to Boost graph library graph. **/
Graph SparseGraphToBgl(const sparsegraph &sg) {
  Graph graph(sg.nv);
  for (int i = 0; i < sg.nv; ++i) {
    int start_index = sg.v[i];
    for (int p = 0; p < sg.d[i]; ++p) {
      int j = sg.e[start_index + p];
      if (j > i) {
        boost::add_edge(i, j, graph);
      }
    }
  }
  return graph;
}

/** Converts Boost graph library graph to sparsegraph. **/
void BglToSparseGraph(const Graph &graph, sparsegraph &sg) {
  const int num_vertices = boost::num_vertices(graph);
  const int num_edges = boost::num_edges(graph);

  // Allocate sparsegraph.
  SG_INIT(sg);
  SG_ALLOC(sg, num_vertices, num_edges * 2, "OutputGraph");
  sg.nv = num_vertices;
  sg.nde = num_edges * 2;

  size_t *vptr = sg.v;
  int *dptr = sg.d;
  int *eptr = sg.e;
  int cum_degree = 0;
  for (int i = 0; i < num_vertices; ++i) {
    *vptr = cum_degree;
    ++vptr;
    int degree = 0;
    for (int j = 0; j < num_vertices; ++j) {
      std::pair<Edge, bool> p = boost::edge(i, j, graph);
      if (p.second) {
        *eptr = j;
        ++eptr;
        ++degree;
        ++cum_degree;
      }
    }
    *dptr = degree;
    ++dptr;
  }
  assert(cum_degree == num_edges * 2);
}
