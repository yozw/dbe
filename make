#!/bin/bash
INCLUDES="nauty"
g++ -std=c++11 -Inauty -lgflags -O3 src/dbe.cc src/graphs.cc src/analysis.cc nauty/gtools.c -o dbe
g++ -std=c++11 -Inauty -O3 src/add_vertex.cc src/graphs.cc nauty/gtools.c -o add_vertex
