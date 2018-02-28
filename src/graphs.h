#ifndef __GRAPHS_H__
#define __GRAPHS_H__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include <gtools.h>

// create a typedef for the Graph type
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::exterior_vertex_property<Graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;
typedef boost::constant_property_map<Edge, int> WeightMap;

/** Reads a non-empty line from stdin. **/
boost::optional<std::string> ReadLine();

/** Reads a sparse6-encoded graph from stdin. **/
boost::optional<Graph> ReadGraph();

/** Writes a graph in sparse6 encoding to stdout. **/
void WriteGraph(const Graph &graph);

/** Converts sparsegraph to Boost graph library graph. **/
Graph SparseGraphToBgl(const sparsegraph &sg);

/** Converts Boost graph library graph to sparsegraph. **/
void BglToSparseGraph(const Graph &graph, sparsegraph &sg);

/** Determines whether the given edge is a bridge. **/
bool IsBridge(const Graph &graph, int u, int v);

/** Calculated the maximum degree of a graph. **/
int MaxDegree(const Graph &graph);

#endif
