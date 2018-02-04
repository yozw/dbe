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

bool SKIP_UNIVERSAL = false; // Don't output graphs with universal lines.
bool SKIP_N_LINES = false;   // Don't output graphs with >= n lines.
bool SKIP_AMRZ = false;      // Don't output graphs that satisfy the Aboulker-Matamala-Rochet-Zamora conjecture.

const int MAX_N = 31;

void GetLine(const Graph &graph, const DistanceMatrixMap &dist, const int i,
             const int j, std::bitset<MAX_N> *line) {
  const int num_vertices = boost::num_vertices(graph);
  assert(i >= 0);
  assert(j >= 0);
  assert(i < num_vertices);
  assert(j < num_vertices);

  line->reset();
  line->set(i);
  line->set(j);

  const int dij = dist[i][j];
  for (int k = 0; k < num_vertices; ++k) {
    if ((k == i) || (k == j)) {
      continue;
    }
    const int dik = dist[i][k];
    const int djk = dist[j][k];

    // i-j-k or j-i-k or i-k-j
    if ((dij + djk == dik) || (dij + dik == djk) || (dik + djk == dij)) {
      line->set(k);
    }
  }
}

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-n") {
      std::cerr << ">dbe will not output graphs with n lines" << std::endl;
      SKIP_N_LINES = true;
    } else if (arg == "-u") {
      std::cerr << ">dbe will not output graphs with universal lines"
                << std::endl;
      SKIP_UNIVERSAL = true;
    } else if (arg == "-z") {
      std::cerr << ">dbe will not output graphs that satisfy the AMRZ conjecture"
                << std::endl;
      SKIP_AMRZ = true;
    } else {
      Error("Invalid argument " + arg);
    }
  }

  std::cerr << ">A dbe" << std::endl;

  auto begin_time = Clock::now();
  int num_graphs = 0;
  int num_output_graphs = 0;

  boost::optional<Graph> optional_graph;
  while ((optional_graph = ReadGraph())) {
    ++num_graphs;

    Graph graph(optional_graph.get());
    const int num_vertices = boost::num_vertices(graph);
    if (num_vertices > MAX_N) {
      Error("Graph too large");
    }

    // Perform Floyd-Warshall all-pairs shortest paths to determine pairwise
    // distances.
    DistanceMatrix distance_matrix(num_vertices);
    DistanceMatrixMap dist(distance_matrix, graph);
    WeightMap weight_map(1);
    floyd_warshall_all_pairs_shortest_paths(graph, distance_matrix,
                                            boost::weight_map(weight_map));

    const unsigned long universal_line = (1L << num_vertices) - 1;

    // Get set of lines.
    std::set<unsigned long> lines;
    std::bitset<MAX_N> line;
    int num_universal = 0;
    for (int i = 0; i < num_vertices; ++i) {
      for (int j = i + 1; j < num_vertices; ++j) {
        GetLine(graph, dist, i, j, &line);
        const unsigned long line_as_long = line.to_ulong();
        lines.insert(line_as_long);
        if (line_as_long == universal_line) {
          ++num_universal;
        }
      }
    }

    // Determine whether to output this graph.
    const bool skip = (SKIP_UNIVERSAL && num_universal > 0) ||
                      (SKIP_N_LINES && lines.size() >= num_vertices) ||
                      (SKIP_AMRZ && lines.size() + num_universal >= num_vertices);

    if (!skip) {
      ++num_output_graphs;
      std::cerr << "Graph " << num_output_graphs << " has " << lines.size()
                << " lines and " << num_universal
                << " universal lines." << std::endl;
      WriteGraph(graph);
    }
  }

  std::cerr << ">Z dbe analyzed " << num_graphs << " graphs in "
            << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
            << std::endl;
  return 0;
}
