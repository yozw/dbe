#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include "graphs.h"

const int MAX_N = 31;

struct AnalysisOptions {
  bool verbose = false;
  bool count_bridges = false;
  bool count_lines_by_distance = false;
  bool include_universal_in_lines = true;
  int dmin;
  int dmax;
  int dumin;
  int dumax;
};

struct GraphInfo {
  int num_lines = 0;
  int num_lines_dist1 = 0;
  int num_lines_dist2 = 0;
  int num_universal = 0;
  int num_universal_dist1 = 0;
  int num_universal_dist2 = 0;
  int num_vertices = 0;
  int num_line_pairs = 0;
  int num_bridges = 0;
  int amrz_gap = 0;
};

bool AnalyzeGraph(const Graph &graph, const AnalysisOptions &options,
                  GraphInfo *graph_info);

#endif
