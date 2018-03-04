/** Adds a vertex in every possible way.

Example:
  NAUTY=../nauty26r11
  $NAUTY/geng -b 6 | ./add_vertex | $NAUTY/shortg

This example generates all 6-vertex bipartite graphs, then generates all ways of
adding a vertex to each of them, and finally removes all isomorphic duplicates.
**/
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <gflags/gflags.h>

#include "common.h"
#include "graphs.h"

DEFINE_int32(t, 1,
             "Type of vertex addition: 1 for all possible ways, "
             "2 for clone, 3 for adjacent clone, 4 for non-adjacent clone");

DEFINE_int32(min_degree, 2, "Minimum degree if resulting vertex");

const unsigned int ALL = 1;
const unsigned int ADJACENT_CLONE = 2;
const unsigned int NON_ADJACENT_CLONE = 3;
const unsigned int CLONE = 4;

int num_graphs = 0;

void ParseCommandLineFlags(int argc, char *argv[]) {
  gflags::SetUsageMessage("Adds vertices to Nauty-generated graphs.");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

void GenerateAllVertexAdditions(const Graph &graph) {
  const unsigned int num_vertices = boost::num_vertices(graph);

  // Generate all possible ways of adding one more vertex.
  const unsigned long num_subsets = 1L << num_vertices;
  for (unsigned long subset = 0; subset < num_subsets; ++subset) {
    Graph new_graph(graph);
    boost::add_vertex(new_graph);
    const unsigned int new_vertex = num_vertices;

    unsigned long mask = 1;
    unsigned int degree = 0;
    for (unsigned int i = 0; i < num_vertices; ++i, mask <<= 1) {
      if ((subset & mask) != 0) {
        boost::add_edge(i, new_vertex, new_graph);
        ++degree;
      }
    }
    if (degree >= FLAGS_min_degree) {
      ++num_graphs;
      WriteGraph(new_graph);
    }
  }
}

void GenerateClones(const Graph &graph, bool adjacent) {
  const unsigned int num_vertices = boost::num_vertices(graph);

  for (unsigned int i = 0; i < num_vertices; ++i) {
    Graph new_graph(graph);
    boost::add_vertex(new_graph);
    const unsigned int clone = num_vertices;
    unsigned int degree = 0;

    if (adjacent) {
      ++degree;
      boost::add_edge(i, clone, new_graph);
    }
    for (unsigned int j = 0; j < num_vertices; ++j) {
      std::pair<Edge, bool> p = boost::edge(i, j, new_graph);
      if (p.second) {
        boost::add_edge(clone, j, new_graph);
        ++degree;
      }
    }
    if (degree >= FLAGS_min_degree) {
      ++num_graphs;
      WriteGraph(new_graph);
    }
  }
}

int main(int argc, char *argv[]) {
  ParseCommandLineFlags(argc, argv);

  auto begin_time = Clock::now();
  std::cerr << ">A add_vertex" << std::endl;

  boost::optional<Graph> optional_graph;
  while ((optional_graph = ReadGraph())) {
    Graph graph = optional_graph.get();

    if (FLAGS_t == ALL) {
      GenerateAllVertexAdditions(graph);
    } else if (FLAGS_t == CLONE) {
      GenerateClones(graph, true);
      GenerateClones(graph, false);
    } else if (FLAGS_t == ADJACENT_CLONE) {
      GenerateClones(graph, true);
    } else if (FLAGS_t == NON_ADJACENT_CLONE) {
      GenerateClones(graph, false);
    }
  }

  auto end_time = Clock::now();
  std::cerr << ">Z add_vertex generated " << num_graphs << " graphs in "
            << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
            << std::endl;
  return 0;
}
