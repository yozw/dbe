/** Graph vertex distance calculator.

Example usage:

    NAUTY="../nauty26r11"
    $NAUTY/geng -b -C 6 | ./g2dist
**/

#include <chrono>
#include <iostream>

#include <gflags/gflags.h>

#include "src/common.h"
#include "src/graphs.h"

DEFINE_bool(q, false, "Quiet mode");

void ParseCommandLineFlags(int argc, char *argv[]) {
  gflags::SetUsageMessage("Distance matrix calculator.");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

int main(int argc, char *argv[]) {
  ParseCommandLineFlags(argc, argv);

  if (!FLAGS_q) {
    std::cerr << ">A g2dist" << std::endl;
  }

  auto begin_time = Clock::now();
  unsigned long long num_graphs = 0;
  boost::optional<Graph> optional_graph;
  while ((optional_graph = ReadGraph())) {
    ++num_graphs;

    Graph graph = optional_graph.get();
    const int num_vertices = boost::num_vertices(graph);
    DistanceMatrix distance_matrix(num_vertices);
    GetDistanceMatrix(graph, &distance_matrix);
    DistanceMatrixMap dist(distance_matrix, graph);
    std::cout << num_vertices << " ";
    for (int i = 0; i < num_vertices; ++i) {
      for (int j = i + 1; j < num_vertices; ++j) {
        std::cout << dist[i][j];
        std::cout << " ";
      }
    }
    WriteGraph(graph);
  }

  if (!FLAGS_q) {
    std::cerr << ">Z g2dist analyzed " << num_graphs << " graphs in "
              << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
              << std::endl;
  }
  gflags::ShutDownCommandLineFlags();
  return 0;
}
