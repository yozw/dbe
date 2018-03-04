#include <bitset>
#include <iostream>

#include "src/analysis.h"
#include "src/common.h"
#include "src/graphs.h"

unsigned long GetLine(const Graph &graph, const DistanceMatrixMap &dist,
                      const int i, const int j) {
  const int num_vertices = boost::num_vertices(graph);
  assert(i >= 0);
  assert(j >= 0);
  assert(i < num_vertices);
  assert(j < num_vertices);

  unsigned long line = 0;
  unsigned long mask = 1;

  auto dist_i = dist[i];
  auto dist_j = dist[j];

  const int dij = dist_i[j];
  for (int k = 0; k < num_vertices; ++k, mask <<= 1) {
    if ((k == i) || (k == j)) {
      line |= mask;
      continue;
    }
    const int dik = dist_i[k];
    const int djk = dist_j[k];

    // i-j-k or j-i-k or i-k-j
    if ((dij + djk == dik) || (dij + dik == djk) || (dik + djk == dij)) {
      line |= mask;
    }
  }
  return line;
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
  std::bitset<(1 << MAX_N)> lines;
  std::bitset<(1 << MAX_N)> lines_dist1;
  std::bitset<(1 << MAX_N)> lines_dist2;
  int num_universal = 0;
  int num_universal_dist1 = 0;
  int num_universal_dist2 = 0;
  int num_line_pairs = 0;
  int num_bridges = 0;
  for (int i = 0; i < num_vertices; ++i) {
    for (int j = i + 1; j < num_vertices; ++j) {
      const int d = dist[i][j];
      if (d > MAX_N) {
        return false; // The graph is disconnected.
      }
      const unsigned long line = GetLine(graph, dist, i, j);
      if (options.verbose) {
        std::bitset<MAX_N> line_bitset(line);
        std::cerr << "d(" << i << "," << j << ") = " << d << "; l(" << i << ","
                  << j << ") = {";
        for (int k = 0; k < num_vertices; ++k) {
          if (line_bitset.test(k)) {
            std::cerr << k << ",";
          }
        }
        std::cerr << "}" << std::endl;
      }
      // Count this line only if we want to include the universal line in the
      // lines or this line is not universal in the first place.
      if (options.include_universal_in_lines || (line != universal_line)) {
        // If dmin <= d <= dmax, count this line.
        if ((d >= options.dmin) && (d <= options.dmax)) {
          ++num_line_pairs;
          lines.set(line);
        }
        // If counting lines by distance is requested, do so here.
        if (options.count_lines_by_distance) {
          if (d == 1) {
            lines_dist1.set(line);
          } else if (d == 2) {
            lines_dist2.set(line);
          }
        }
      }
      // If counting bridges is requested, do so.
      if (options.count_bridges && (d == 1) && IsBridge(graph, i, j)) {
        ++num_bridges;
      }
      // If this line is the universal line, do additional counting.
      if (line == universal_line) {
        if (options.skip_graphs_with_universal_line) {
          return false;
        }
        // If umin <= d <= umax, count this pair as generating the universal
        // line.
        if ((d >= options.dumin) && (d <= options.dumax)) {
          ++num_universal;
          // Count pairs generating the universal line by distance.
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
  info->num_lines = lines.count();
  info->num_lines_dist1 = lines_dist1.count();
  info->num_lines_dist2 = lines_dist2.count();
  info->num_universal = num_universal;
  info->num_universal_dist1 = num_universal_dist1;
  info->num_universal_dist2 = num_universal_dist2;
  info->num_line_pairs = num_line_pairs;
  info->num_bridges = num_bridges;
  info->amrz_gap = info->num_lines + info->num_universal - info->num_vertices;
  return true;
}
