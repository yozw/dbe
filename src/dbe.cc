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

struct GraphInfo {
  int num_lines;
  int num_universal;
  int num_universal_dist1;
  int num_universal_dist2;
  int num_vertices;
  int num_line_pairs;
};

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

void AnalyzeGraph(const Graph& graph, GraphInfo* graph_info) {
  const int num_vertices = boost::num_vertices(graph);
  if (num_vertices > MAX_N) {
    Error("Graph too large");
  }
  const unsigned long universal_line = (1L << num_vertices) - 1;

  // Perform Floyd-Warshall all-pairs shortest paths to determine pairwise
  // distances.
  DistanceMatrix distance_matrix(num_vertices);
  DistanceMatrixMap dist(distance_matrix, graph);
  WeightMap weight_map(1);
  floyd_warshall_all_pairs_shortest_paths(graph, distance_matrix,
                                          boost::weight_map(weight_map));

  // Get set of lines.
  std::set<unsigned long> lines;
  std::bitset<MAX_N> line;
  int num_universal = 0;
  int num_universal_dist1 = 0;
  int num_universal_dist2 = 0;
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
          if (d == 1) {
            ++num_universal_dist1;
          } else if (d == 2) {
            ++num_universal_dist2;
          }
        }
      }
    }
  }

  graph_info->num_vertices = num_vertices;
  graph_info->num_lines = lines.size();
  graph_info->num_universal = num_universal;
  graph_info->num_universal_dist1 = num_universal_dist1;
  graph_info->num_universal_dist2 = num_universal_dist2;
  graph_info->num_line_pairs = num_line_pairs;
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
    GraphInfo info;
    AnalyzeGraph(graph, &info);

    // Determine whether to output this graph.
    const int amrz_gap = info.num_lines + info.num_universal - info.num_vertices;
    const bool skip = (FLAGS_u && info.num_universal > 0) ||
                      (FLAGS_n && info.num_lines >= info.num_vertices) ||
                      (amrz_gap < FLAGS_zmin) ||
                      (amrz_gap > FLAGS_zmax);

    if (!skip) {
      ++num_output_graphs;
      switch (FLAGS_o) {
      case 0: {
        std::cerr << "Graph " << num_output_graphs << " has " << info.num_lines
                  << " lines (from " << info.num_line_pairs << " pairs) and " << info.num_universal
                  << " universal lines (AMRZ gap " << amrz_gap << ")"
                  << std::endl;
        WriteGraph(graph);
        break;
      }
      case 1:
        std::cout << info.num_lines << "," << info.num_universal << "," << amrz_gap << std::endl;
      }
    }
  }

  std::cerr << ">Z dbe analyzed " << num_graphs << " graphs in "
            << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
            << std::endl;

  gflags::ShutDownCommandLineFlags();
  return 0;
}
