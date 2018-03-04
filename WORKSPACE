new_http_archive(
  name='nauty',
  urls=['http://www.yoriz.co.uk/nauty26r11.tar.gz',
  'http://users.cecs.anu.edu.au/~bdm/nauty/nauty26r11.tar.gz'],
  build_file='BUILD.nauty',
  strip_prefix='nauty26r11'
)

git_repository(
    name   = "com_github_gflags_gflags",
    commit = "e292e0452fcfd5a8ae055b59052fc041cbab4abf",
    remote = "https://github.com/gflags/gflags.git",
)

bind(
    name = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)
