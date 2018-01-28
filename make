#!/bin/bash
NAUTY="../nauty26r11"
INCLUDES=$NAUTY
#g++ -Wall -O3 src/dbe.cc -o dbe
g++ -g -std=c++11 -I$INCLUDES -O3 src/dbe.cc $NAUTY/gtools.c -o dbe
g++ -g -std=c++11 -I$INCLUDES -O3 src/add_vertex.cc $NAUTY/gtools.c -o add_vertex
