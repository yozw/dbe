load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
  name='nauty',
  urls=[
    # 'http://www.yoriz.co.uk/nauty26r11.tar.gz',
    'http://users.cecs.anu.edu.au/~bdm/nauty/nauty26r11.tar.gz'
  ],
  build_file='BUILD.nauty',
  strip_prefix='nauty26r11',
  sha256='847159e22276103bd3bc0cb1e43ee27ebc7580dcddd144cb42730c1435871834',
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

http_archive(
    name = "boost",
    urls = ["https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz"],
    sha256 = "f55c340aa49763b1925ccf02b2e83f35fdcf634c9d5164a2acb87540173c741d",
    build_file='BUILD.boost',
    strip_prefix='boost_1_87_0'
)
