cmake = import('cmake')

opendis = cmake.subproject('opendis')
poco = cmake.subproject('poco')

deps = [
  opendis.dependency('OpenDIS6'),
  poco.dependency('Net'),
  fuse_dep
]

src = files(
  'protocol.cpp',
  'main.cpp',
)

distest_exe = executable('distest', src, dependencies: deps)
