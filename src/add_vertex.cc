#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common.h"
#include "graphs.h"

const bool MIN_DEGREE = 2;


int main(int, char*[]) {
  auto begin_time = Clock::now();
  int num_graphs = 0;

  while (true) {
    boost::optional<Graph> optional_graph = ReadGraph();
    if (!optional_graph) {
      break;
    }

    const unsigned int num_vertices = boost::num_vertices(optional_graph.get());

    // Generate all possible ways of adding one more vertex.
    const unsigned long num_subsets = 1L << num_vertices;
    for (unsigned long subset = 0; subset < num_subsets; ++subset) {
      Graph graph(optional_graph.get());
      boost::add_vertex(graph);

      unsigned long mask = 1;
      unsigned int degree = 0;
      for (unsigned int i = 0; i < num_vertices; ++i, mask <<= 1) {
        if ((subset & mask) != 0) {
          boost::add_edge(i, num_vertices, graph);
          ++degree;
        }
      }
      if (degree >= MIN_DEGREE) {
        OutputGraph(++num_graphs, graph);
      }
    }
  }

  auto end_time = Clock::now();
  std::cerr << "> Generated " << num_graphs << " graphs in " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() / 1000.0
              << " seconds" << std::endl;
  return 0;
} 
