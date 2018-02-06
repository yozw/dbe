/** De Bruijn-Erdos checker for Nauty-generated graphs.

Example usage:

    NAUTY="../nauty26r11"
    $NAUTY/geng -b -C 6 | ./dbe | $NAUTY/showg -A
**/

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <gflags/gflags.h>

#include "analysis.h"
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

void ParseCommandLineFlags(int argc, char *argv[]) {
  gflags::SetUsageMessage(
      "De Bruijn-Erdos checker for Nauty-generated graphs.");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

std::string Potential(const GraphInfo &info, int constant_term) {
  std::stringstream ss;

  ss << (constant_term + info.num_lines);
  ss << " + " << info.num_universal_dist1 << "*x[1]";
  ss << " + " << info.num_universal_dist2 << "*x[2]";
  ss << " + " << indicator(info.num_universal_dist1) << "*x[3]";
  ss << " + " << indicator(info.num_universal_dist2) << "*x[4]";
  ss << " + " << sqr(info.num_universal_dist1) << "*x[5]";
  ss << " + " << sqr(info.num_universal_dist2) << "*x[6]";
  ss << " + " << info.num_universal << "*x[7]";
  ss << " + " << sqr(info.num_universal) << "*x[8]";
  ss << " + " << info.num_bridges << "*x[9]";
  return ss.str();
}

int main(int argc, char *argv[]) {
  ParseCommandLineFlags(argc, argv);

  std::cerr << ">A dbe" << std::endl;

  AnalysisOptions options;
  options.nmin = FLAGS_nmin;
  options.nmax = FLAGS_nmax;
  options.umin = FLAGS_umin;
  options.umax = FLAGS_umax;
  options.count_bridges = (FLAGS_o == 2);

  auto begin_time = Clock::now();
  int num_graphs = 0;
  int num_output_graphs = 0;
  boost::optional<Graph> optional_graph;
  while ((optional_graph = ReadGraph())) {
    ++num_graphs;

    Graph graph(optional_graph.get());
    GraphInfo info;
    AnalyzeGraph(graph, options, &info);

    // Determine whether to output this graph.
    if (FLAGS_u && info.num_universal > 0) {
      // Skip because this graph has a universal line.
      continue;
    } else if (FLAGS_n && info.num_lines >= info.num_vertices) {
      // Skip because this graph has as many lines as vertices.
      continue;
    } else if ((info.amrz_gap < FLAGS_zmin) || (info.amrz_gap > FLAGS_zmax)) {
      // Skip because this graph an AMRZ gap outside the requested range.
      continue;
    }

    ++num_output_graphs;
    if (FLAGS_o == 0) {
      std::cerr << "Graph " << num_output_graphs << " has " << info.num_lines
                << " lines (from " << info.num_line_pairs << " pairs) and "
                << info.num_universal << " universal lines (AMRZ gap "
                << info.amrz_gap << ")" << std::endl;
      WriteGraph(graph);
    } else if (FLAGS_o == 1) {
      std::cout << info.num_lines << "," << info.num_universal << ","
                << info.amrz_gap << std::endl;
    } else if (FLAGS_o == 2) {
      for (int i = 0; i < info.num_vertices; ++i) {
        Graph subGraph(graph);
        boost::clear_vertex(i, subGraph);
        boost::remove_vertex(i, subGraph);
        GraphInfo subGraphInfo;
        if (AnalyzeGraph(subGraph, options, &subGraphInfo)) {
          std::cout << Potential(info, 0)
                    << " >= " << Potential(subGraphInfo, 1) << std::endl;
        }
      }
    }
  }

  std::cerr << ">Z dbe analyzed " << num_graphs << " graphs in "
            << GetMillisecondsSince(begin_time) / 1000.0 << " seconds"
            << std::endl;

  gflags::ShutDownCommandLineFlags();
  return 0;
}
