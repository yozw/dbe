# Mixed integer linear optimization model to find a "distance" matrix
# corresponding to the "nasty hypergraph".
#
# To solve with CPLEX:
#
# $ glpsol --math nasty.lp --wlp nasty.cplex
# $ cplex
# > read nasty.cplex lp
# > solve

set V;                             # Set of vertices.
set V2 := V cross V;               # V x V (cardinality |V|^2).
set V3 := V cross V cross V;       # V x V x V (cardinality |V|^3).
set H within V3;                   # Hyperedges.
set NH within V3;                  # Non-hyperedges.

param M;
param eps;

# ------ Variable definitions -------
var d{V2} >= 1, integer;         # "Distance" between pairs of points.
var le{V3}, binary;     # Whether d[x, y] + d[y, z] <= d[x, z] (0=no, 1=yes)
var ge{V3}, binary;     # Whether d[x, y] + d[y, z] >= d[x, z] (0=no, 1=yes)
var eq{V3}, binary;     # Whether d[x, y] + d[y, z] = d[x, z]  (0=no, 1=yes)

var max_distance;

# ------ Objective ------
minimize objective: max_distance;

# ------ Define max_distance -------
subject to obj_def {(x, y) in V2: x != y}:
  max_distance >= d[x, y];

# ------ Symmetries -------
# Symmetries in d.
subject to d_symmetry {(x, y) in V2: x != y}:
  d[x, y] = d[y, x];

# Symmetries in le.
subject to le_symmetry {(x, y, z) in V3: x != y and x != z and y != z}:
  le[x, y, z] = le[z, y, x];

# Symmetries in ge.
subject to ge_symmetry {(x, y, z) in V3: x != y and x != z and y != z}:
  ge[x, y, z] = ge[z, y, x];

# Symmetries in eq.
subject to eq_symmetry {(x, y, z) in V3: x != y and x != z and y != z}:
  eq[x, y, z] = eq[z, y, x];

# ------ Relationship between le and d -------
# le[x, y, z] = 1 => d[x, y] + d[y, z] <= d[x, y].
# le[x, y, z] = 0 implies a vacuous constraint.
subject to le1 {(x, y, z) in V3: x != y and x != z and y != z}:
  d[x, y] + d[y, z] <= d[x, z] + M * (1 - le[x, y, z]);

# le[x, y, z] = 0 => d[x, y] + d[y, z] >= d[x, y] + eps.
# le[x, y, z] = 1 implies a vacuous constraint.
subject to le2 {(x, y, z) in V3: x != y and x != z and y != z}:
  d[x, y] + d[y, z] >= d[x, z] - M * le[x, y, z] + eps;

# ------ Relationship between ge and d -------
# ge[x, y, z] = 1 => d[x, y] + d[y, z] >= d[x, y].
# ge[x, y, z] = 0 implies a vacuous constraint.
subject to ge1 {(x, y, z) in V3: x != y and x != z and y != z}:
  d[x, y] + d[y, z] >= d[x, z] - M * (1 - ge[x, y, z]);

# ge[x, y, z] = 0 => d[x, y] + d[y, z] <= d[x, y] - eps.
# ge[x, y, z] = 1 implies a vacuous constraint.
subject to ge2 {(x, y, z) in V3: x != y and x != z and y != z}:
  d[x, y] + d[y, z] <= d[x, z] + M * ge[x, y, z] - eps;

# ------ Relationship between eq, le, and ge -------
# eq[x, y, z] = 1 => le[x, y, z] + ge[x, y, z] = 2.
# eq[x, y, z] = 0 => le[x, y, z] + ge[x, y, z] = 1.
subject to eq_def {(x, y, z) in V3: x != y and x != z and y != z}:
  le[x, y, z] + ge[x, y, z] = eq[x, y, z] + 1;

# ------ Requirements for hyperedges in the hypergraph -------
subject to hyperedges {(x, y, z) in H}:
  eq[x, y, z] + eq[y, x, z] + eq[x, z, y] >= 1;

# ------ Requirements for missing hyperedges in the hypergraph -------
subject to nonhyperedges {(x, y, z) in NH}:
  eq[x, y, z] + eq[y, x, z] + eq[x, z, y] = 0;

data;

param M := 1000;
param eps := 0.1;

# Vertex set.
set V := H V rx ry rz sx sy sz tx ty tz;

# Hyperedges in the hypergraph.
set H :=
 (H, rx, ry)  (H, rx, rz)  (H, ry, rz)  (H, sx, sy)  (H, sx, sz)  (H, sy, sz)  (H, tx, ty)  (H, tx, tz)  (H, ty, tz)
 (V, rx, sx)  (V, rx, tx)  (V, ry, sy)  (V, ry, ty)  (V, rz, sz)  (V, rz, tz)  (V, sx, tx)  (V, sy, ty)  (V, sz, tz)
 (rx, ry, rz)  (rx, ry, sx)  (rx, ry, sy)  (rx, ry, sz)  (rx, ry, tx)  (rx, ry, ty)  (rx, ry, tz)  (rx, rz, sx)  (rx, rz, sy)
 (rx, rz, sz)  (rx, rz, tx)  (rx, rz, ty)  (rx, rz, tz)  (rx, sx, sy)  (rx, sx, sz)  (rx, sx, tx)  (rx, sx, ty)  (rx, sx, tz)
 (rx, sy, sz)  (rx, sy, tx)  (rx, sy, ty)  (rx, sy, tz)  (rx, sz, tx)  (rx, sz, ty)  (rx, sz, tz)  (rx, tx, ty)  (rx, tx, tz)
 (rx, ty, tz)  (ry, rz, sx)  (ry, rz, sy)  (ry, rz, sz)  (ry, rz, tx)  (ry, rz, ty)  (ry, rz, tz)  (ry, sx, sy)  (ry, sx, sz)
 (ry, sx, tx)  (ry, sx, ty)  (ry, sx, tz)  (ry, sy, sz)  (ry, sy, tx)  (ry, sy, ty)  (ry, sy, tz)  (ry, sz, tx)  (ry, sz, ty)
 (ry, sz, tz)  (ry, tx, ty)  (ry, tx, tz)  (ry, ty, tz)  (rz, sx, sy)  (rz, sx, sz)  (rz, sx, tx)  (rz, sx, ty)  (rz, sx, tz)
 (rz, sy, sz)  (rz, sy, tx)  (rz, sy, ty)  (rz, sy, tz)  (rz, sz, tx)  (rz, sz, ty)  (rz, sz, tz)  (rz, tx, ty)  (rz, tx, tz)
 (rz, ty, tz)  (sx, sy, sz)  (sx, sy, tx)  (sx, sy, ty)  (sx, sy, tz)  (sx, sz, tx)  (sx, sz, ty)  (sx, sz, tz)  (sx, tx, ty)
 (sx, tx, tz)  (sx, ty, tz)  (sy, sz, tx)  (sy, sz, ty)  (sy, sz, tz)  (sy, tx, ty)  (sy, tx, tz)  (sy, ty, tz)  (sz, tx, ty)
 (sz, tx, tz)  (sz, ty, tz)  (tx, ty, tz) ;

# Missing hyperedges in the hypergraph.
set NH :=
 (H, V, rx)  (H, V, ry)  (H, V, rz)  (H, V, sx)  (H, V, sy)  (H, V, sz)  (H, V, tx)  (H, V, ty)  (H, V, tz)
 (H, rx, sx)  (H, rx, sy)  (H, rx, sz)  (H, rx, tx)  (H, rx, ty)  (H, rx, tz)  (H, ry, sx)  (H, ry, sy)  (H, ry, sz)
 (H, ry, tx)  (H, ry, ty)  (H, ry, tz)  (H, rz, sx)  (H, rz, sy)  (H, rz, sz)  (H, rz, tx)  (H, rz, ty)  (H, rz, tz)
 (H, sx, tx)  (H, sx, ty)  (H, sx, tz)  (H, sy, tx)  (H, sy, ty)  (H, sy, tz)  (H, sz, tx)  (H, sz, ty)  (H, sz, tz)
 (V, rx, ry)  (V, rx, rz)  (V, rx, sy)  (V, rx, sz)  (V, rx, ty)  (V, rx, tz)  (V, ry, rz)  (V, ry, sx)  (V, ry, sz)
 (V, ry, tx)  (V, ry, tz)  (V, rz, sx)  (V, rz, sy)  (V, rz, tx)  (V, rz, ty)  (V, sx, sy)  (V, sx, sz)  (V, sx, ty)
 (V, sx, tz)  (V, sy, sz)  (V, sy, tx)  (V, sy, tz)  (V, sz, tx)  (V, sz, ty)  (V, tx, ty)  (V, tx, tz)  (V, ty, tz)
;

end;
