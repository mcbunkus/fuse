cmake = import('cmake')
poco = cmake.subproject('poco')

deps = [
  poco.dependency('Util'),
  poco.dependency('Foundation'),
]

include = include_directories('api')

src = files(
  'src/ini_config.cpp',
)

fuse_lib = library('fuse', src, include_directories: include, dependencies: deps)
fuse_dep = declare_dependency(link_with: fuse_lib, include_directories: include)

subdir('tst')
