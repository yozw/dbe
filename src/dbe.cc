/** De Bruijn-Erdos checker for Nauty-generated graphs.

Example usage:

    NAUTY="../nauty26r11"
    $NAUTY/geng -b -C 6 | ./dbe | $NAUTY/showg -A
**/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common.h"
#include "graphs.h"

const bool SKIP_UNIVERSAL = false;  // Don't output graphs with universal lines.
const bool SKIP_N_LINES = true;  // Don't output graphs with >= n lines.


void GetLine(const Graph& graph, const DistanceMatrixMap& dist, const int i, const int j, std::set<int> *line) {
  const int num_vertices = boost::num_vertices(graph);
  assert(i >= 0);
  assert(j >= 0);
  assert(i < num_vertices);
  assert(j < num_vertices);

  line->clear();
  line->insert(i);
  line->insert(j);

  const int dij = dist[i][j];
  for (int k = 0; k < num_vertices; ++k) {
    if ((k == i) || (k == j)) {
      continue;
    }
    const int dik = dist[i][k];
    const int djk = dist[j][k];

    // i-j-k or j-i-k or i-k-j
    if ((dij + djk == dik) || (dij + dik == djk) || (dik + djk == dij)) {
      line->insert(k);
    }
  }
}

int main(int, char*[]) {
  auto begin_time = Clock::now();
  int num_graphs = 0;
  int num_output_graphs = 0;

  boost::optional<Graph> optional_graph;
  while ((optional_graph = ReadGraph())) {
    ++num_graphs;

    Graph graph(optional_graph.get());
    const int num_vertices = boost::num_vertices(graph);

    // Perform Floyd-Warshall all-pairs shortest paths to determine pairwise distances.
    DistanceMatrix distance_matrix(num_vertices);
    DistanceMatrixMap dist(distance_matrix, graph);
    WeightMap weight_map(1);
    floyd_warshall_all_pairs_shortest_paths(graph, distance_matrix, boost::weight_map(weight_map));

    // Calculate number of lines and whether the graph has a universal line.
    bool has_universal = false;
    std::set<std::set<int>> lines;
    for (int i = 0; i < num_vertices; ++i) {
      for (int j = i + 1; j < num_vertices; ++j) {
        std::set<int> line;
        GetLine(graph, dist, i, j, &line);
        lines.insert(line);
        if (line.size() == num_vertices) {
          has_universal = true;
        }
      }
    }

    // Determine whether to output this graph.
    bool should_output = true;
    if (SKIP_UNIVERSAL) {
      should_output &= !has_universal;
    }
    if (SKIP_N_LINES) {
      should_output &= lines.size() < num_vertices;
    }

    if (should_output) {
      ++num_output_graphs;
      std::cerr << "Graph " << num_output_graphs << " has " << lines.size() << " lines and " << (has_universal ? "a" : "no")
        << " universal line." << std::endl;
      WriteGraph(graph);
    }
  }

  auto end_time = Clock::now();
  std::cerr << ">dbe analyzed " << num_graphs << " graphs in " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count() / 1000.0
              << " seconds" << std::endl;
  return 0;
} 
