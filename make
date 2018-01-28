#!/bin/bash
NAUTY="../nauty26r11"
INCLUDES=$NAUTY
g++ -std=c++11 -I$INCLUDES -O3 src/dbe.cc src/graphs.cc $NAUTY/gtools.c -o dbe
g++ -std=c++11 -I$INCLUDES -O3 src/add_vertex.cc src/graphs.cc $NAUTY/gtools.c -o add_vertex
