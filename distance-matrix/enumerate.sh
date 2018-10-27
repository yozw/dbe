#!/bin/bash
# Enumerates all isomorphically distinct (pseudo-metric) spaces with given distances.
# They are pseudo-metric in the sense that the triangle inequality is not
# necessarily respected.
#
# This is done by generating the line graph of K_n, generating all vertex-colorings
# of this graph, and then mapping those colorings back to edge-colorings of K_n, with
# colors replaced by the requested distances.
BASE=$(dirname $0)/..
NAUTY=${BASE}/../nauty26r11

# Number of points.
N="$1"

shift
DISTANCES="$*"

if [[ "$DISTANCES" == "" ]]; then
  echo "Syntax: enumerate-234 <N> <DISTANCES>" > /dev/stderr
  exit 1
fi

NLABELS=0
for x in $DISTANCES
do
  NLABELS=$[$NLABELS +1]
done

echo ">A Generating all vertex-${NLABELS}-colorings of LG(K_${N})." > /dev/stderr
python ${BASE}/distance-matrix/linegraph-kn.py ${N} | ${NAUTY}/amtog | ${NAUTY}/vcolg -T -m${NLABELS} \
    | ${BASE}/bazel-bin/vc2dist ${DISTANCES}
