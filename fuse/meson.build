include = include_directories('api')

src = files(
  'src/configuration/ini.cpp',
  'src/configuration/configuration.cpp',
  'src/util.cpp',
)

fuse_lib = library('fuse', src, include_directories: include)
fuse_dep = declare_dependency(link_with: fuse_lib, include_directories: include)

subdir('tst')
