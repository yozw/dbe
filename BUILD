cc_binary(
  name='dbe',
  srcs=['dbe.cc'],
  deps=[':analysis', ':graphs', '//external:gflags']
)

cc_binary(
  name='add_vertex',
  srcs=['add_vertex.cc'],
  deps=[':graphs', '//external:gflags']
)

cc_binary(
  name='calc',
  srcs=['calc.cc'],
)

cc_library(
  name='analysis',
  srcs=['analysis.cc'],
  hdrs=['analysis.h'],
  deps=[':graphs']
)

cc_library(
  name='graphs',
  srcs=['graphs.cc'],
  hdrs=['graphs.h'],
  deps=[':common', '@nauty//:headers'],
  copts=['-Inauty']
)

cc_library(
  name='common',
  hdrs=['common.h']
)

