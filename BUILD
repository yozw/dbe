cc_binary(
  name='dbe',
  srcs=['src/dbe.cc'],
  deps=[':analysis', ':graphs', '//external:gflags']
)

cc_binary(
  name='add_vertex',
  srcs=['src/add_vertex.cc'],
  deps=[':graphs_adj', '//external:gflags'],
  copts=['-DUSE_ADJACENCY_LIST']
)

cc_binary(
  name='calc',
  srcs=['src/calc.cc'],
)

cc_library(
  name='analysis',
  srcs=['src/analysis.cc'],
  hdrs=['src/analysis.h'],
  deps=[':graphs']
)

cc_library(
  name='graphs',
  srcs=['src/graphs.cc'],
  hdrs=['src/graphs.h'],
  deps=[':common', '@nauty//:headers', '@nauty//:gtools'],
  copts=['-Inauty']
)

cc_library(
  name='graphs_adj',
  srcs=['src/graphs.cc'],
  hdrs=['src/graphs.h'],
  deps=[':common', '@nauty//:headers', '@nauty//:gtools'],
  copts=['-Inauty', '-DUSE_ADJACENCY_LIST']
)

cc_library(
  name='common',
  hdrs=['src/common.h']
)

