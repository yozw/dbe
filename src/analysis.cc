
#include "analysis.h"
#include "common.h"
#include "graphs.h"

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

bool AnalyzeGraph(const Graph &graph, const AnalysisOptions &options,
                  GraphInfo *info) {
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
  int num_bridges = 0;
  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      GetLine(graph, dist, i, j, &line);
      const unsigned long line_as_long = line.to_ulong();
      const int d = dist[i][j];
      if (d > MAX_N) {
        return false; // The graph is disconnected.
      }
      if (options.count_bridges && (d == 1) && IsBridge(graph, i, j)) {
        ++num_bridges;
      }
      if ((d >= options.nmin) && (d <= options.nmax)) {
        ++num_line_pairs;
        lines.insert(line_as_long);
      }
      if (line_as_long == universal_line) {
        if ((d >= options.umin) && (d <= options.umax)) {
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

  info->num_vertices = num_vertices;
  info->num_lines = lines.size();
  info->num_universal = num_universal;
  info->num_universal_dist1 = num_universal_dist1;
  info->num_universal_dist2 = num_universal_dist2;
  info->num_line_pairs = num_line_pairs;
  info->num_bridges = num_bridges;
  info->amrz_gap = info->num_lines + info->num_universal - info->num_vertices;
  return true;
}
