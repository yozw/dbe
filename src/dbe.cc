/** De Bruijn-Erdos checker for Nauty-generated graphs.

Example usage:

    NAUTY="../nauty26r11"
    $NAUTY/geng -b -C 6 | ./dbe | $NAUTY/showg -A
**/

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <gflags/gflags.h>

#include "common.h"
#include "graphs.h"

DEFINE_bool(n, false, "Do not output graphs with |V(G)| lines");
DEFINE_int32(nmin, 0,
             "Only count lines induced by vertices at at least this distance");
DEFINE_int32(nmax, INT_MAX,
             "Only count lines induced by vertices at at "
             "least this distance");
DEFINE_bool(u, false, "Do not output graphs with a universal line");
DEFINE_int32(umin, 0,
             "Only count universal lines induced by vertices "
             "at at least this distance");
DEFINE_int32(
    umax, INT_MAX,
    "Only count universal lines induced by vertices at at least this distance");
DEFINE_int32(zmin, INT_MIN,
             "Only output graphs what a AMRZ gap of at least this number");
DEFINE_int32(zmax, INT_MAX,
             "Only output graphs what a AMRZ gap of at most this number");
DEFINE_int32(o, 0, "Output format");

const int MAX_N = 31;

void ParseCommandLineFlags(int argc, char *argv[]) {
  gflags::SetUsageMessage(
      "De Bruijn-Erdos checker for Nauty-generated graphs.");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

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
  ParseCommandLineFlags(argc, argv);

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
    int num_line_pairs = 0;
    for (int i = 0; i < num_vertices; ++i) {
      for (int j = i + 1; j < num_vertices; ++j) {
        GetLine(graph, dist, i, j, &line);
        const unsigned long line_as_long = line.to_ulong();
        const int d = dist[i][j];
        if ((d >= FLAGS_nmin) && (d <= FLAGS_nmax)) {
          ++num_line_pairs;
          lines.insert(line_as_long);
        }
        if (line_as_long == universal_line) {
          if ((d >= FLAGS_umin) && (d <= FLAGS_umax)) {
            ++num_universal;
          }
        }
      }
    }

    // Determine whether to output this graph.
    const int amrz_sum = lines.size() + num_universal;
    const int amrz_gap = amrz_sum - num_vertices;
    const bool skip = (FLAGS_u && num_universal > 0) ||
                      (FLAGS_n && lines.size() >= num_vertices) ||
                      (amrz_gap < FLAGS_zmin) ||
                      (amrz_gap > FLAGS_zmax);

    if (!skip) {
      ++num_output_graphs;
      switch (FLAGS_o) {
      case 0: {
        std::cerr << "Graph " << num_output_graphs << " has " << lines.size()
                  << " lines (from " << num_line_pairs << " pairs) and " << num_universal
                  << " universal lines (AMRZ gap " << amrz_gap << ")"
                  << std::endl;
        WriteGraph(graph);
        break;
      }
      case 1:
        std::cout << lines.size() << "," << num_universal << "," << amrz_gap << std::endl;
      }
    }
  }

  std::cerr << ">Z dbe analyzed " << num_graphs << " graphs in "
            << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
            << std::endl;

  gflags::ShutDownCommandLineFlags();
  return 0;
}
