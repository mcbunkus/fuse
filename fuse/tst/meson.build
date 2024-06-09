# FUSE unit tests

# put test files here
tests = [
  'channel.cpp',
  'threadpool.cpp',
  'configuration/ini.cpp',
]

foreach _test : tests
  name = _test.split('.')[0].replace('/', '.')
  exe = executable(name, _test, dependencies: fuse_dep)
  test(name, exe)
endforeach
