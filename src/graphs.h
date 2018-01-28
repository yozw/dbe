#include <boost/algorithm/string.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include <gtools.h>


// create a typedef for the Graph type
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::exterior_vertex_property<Graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;
typedef boost::constant_property_map<Edge, int> WeightMap;

typedef std::chrono::high_resolution_clock Clock;


inline boost::optional<std::string> ReadLine() {
  std::string line;
  do {
    if (std::cin.eof()) {
      return boost::none;
    }
    std::getline(std::cin, line);
  } while (line.length() == 0);
  return line;  
}

inline boost::optional<Graph> ReadGraph() {
  boost::optional<std::string> line = ReadLine();
  if (!line) {
    return boost::none;
  }
  sparsegraph sg;
  SG_INIT(sg);
  int nloops;
  char* repr = const_cast<char*>(line.get().c_str());
  stringtosparsegraph(repr, &sg, &nloops);
  if (nloops != 0) {
    Error("Loops are not supported");
  }

  Graph graph(sg.nv);
  for (int i = 0; i < sg.nv; ++i) {
    int start_index = sg.v[i];
    for (int p = 0; p < sg.d[i]; ++p) {
      int j = sg.e[start_index + p];
      if (j > i) {
        boost::add_edge(i, j, graph);
      }
    }
  }
  return graph;
}

inline boost::optional<Graph> ReadGraph2() {
  boost::optional<std::string> header = ReadLine();
  if (!header) {
    return boost::none;
  }

  std::vector<std::string> tokens;
  boost::split(tokens, header.get(), boost::is_any_of("\t "));

  if (tokens[0] != "Graph") {
    Error("Expected a graph");
  }

  int num_vertices = std::stoi(tokens[3]);
  if (num_vertices <= 0) {
    Error("Expected positive number of vertices");
  }

  Graph graph(num_vertices);
  for (int i = 0; i < num_vertices; ++i) {
    std::vector<std::string> adj;
    boost::optional<std::string> line = ReadLine();
    if (!line) {
      Error("EOF while reading adjacency");
    }
    boost::split(adj, line.get(), boost::is_any_of("\t "));
    if (adj.size() != num_vertices) {
      Error("Incorrect adjacency list size");
    }
    for (int j = i + 1; j < num_vertices; ++j) {
      if (adj[j] == "1") {
        boost::add_edge(i, j, graph);
      }
    }
  }
  return graph;
}

inline void OutputGraph(const int index, const Graph& graph) {
  const int num_vertices = boost::num_vertices(graph);
  const int num_edges = boost::num_edges(graph);
  sparsegraph sg;
  SG_INIT(sg);
  SG_ALLOC(sg, num_vertices, num_edges * 2, "OutputGraph");
  sg.nv = num_vertices;
  sg.nde = num_edges * 2;

  size_t* vptr = sg.v;
  int* dptr = sg.d;
  int* eptr = sg.e;
  int cum_degree = 0;
  for (int i = 0; i < num_vertices; ++i) {
    *vptr = cum_degree;
    ++vptr;
    int degree = 0;
    for (int j = 0; j < num_vertices; ++j) {
      std::pair<Edge, bool> p = boost::edge(i, j, graph);
      if (p.second) {
        *eptr = j;
        ++eptr;
        ++degree;
        ++cum_degree;
      }
    }
    *dptr = degree;
    ++dptr;
  }
  assert(cum_degree == num_edges * 2);
  char* sgraph6 = sgtos6(&sg);
  std::cout << sgraph6;
  SG_FREE(sg);
}
