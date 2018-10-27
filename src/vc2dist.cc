/** Vertex coloring to distance matrix mapper.

Example usage:

    python distance-matrix/linegraph-kn.py 5 | ${NAUTY}/amtog \
        | ${NAUTY}/vcolg -T -m3 | ./vc2dist 2 3 4
**/

#include "common.h"
#include "graphs.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

void ParseArguments(int argc, char* argv[], std::vector<int>* distance_map) {
  if (argc < 3) {
    std::cerr << "Syntax: vc2dist <DISTANCES>" << std::endl;
    exit(1);
  }
  distance_map->clear();
  for (int i = 1; i < argc; ++i) {
    distance_map->push_back(atoi(argv[i]));
  }
}

int main(int argc, char *argv[]) {
  std::vector<int> distance_map;
  ParseArguments(argc, argv, &distance_map);

  std::cerr << ">A Mapping vertex coloring of LG(K_n) to edge distances ";
  for (int i = 0; i < distance_map.size(); ++i) {
    if (i != 0) {
      std::cerr << ", ";
    }
    std::cerr << distance_map[i];
  }
  std::cerr << "." << std::endl;

  int index = 0;
  boost::optional<std::string> line;
  
  while ((line = ReadLine())) {
    std::stringstream stream(line.get());
    int num_edges;
    stream >> num_edges;
    int n = int(round(0.5 * (1.0 + sqrt(1.0 + 8 * num_edges))));
    if (n * (n - 1) / 2 != num_edges) {
      Error("Invalid number of edges: " + std::to_string(num_edges));
    }
    int unused_num;
    stream >> unused_num;
    std::cout << n;
    for (int i = 0; i < num_edges; ++i) {
      int label;
      stream >> label;
      if (label < 0 || label >= distance_map.size()) {
        Error("Invalid label: " + std::to_string(label));
      }
      int distance = distance_map[label];
      std::cout << " " << distance;
    }
    std::cout << " matrix-" << n << "-" << index << std::endl;
    ++index;
  }
}
