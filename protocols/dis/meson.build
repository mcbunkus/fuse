cmake = import('cmake')

opendis = cmake.subproject('opendis')

deps = [
  opendis.dependency('OpenDIS6'),
  fuse_dep
]

src = files(
  'protocol.cpp',
  'main.cpp',
)

distest_exe = executable('distest', src, dependencies: deps)
