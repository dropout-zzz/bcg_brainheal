cmake_minimum_required(VERSION 4.2.0)

project(Bcg LANGUAGES C)

add_executable(scan_test
  scan/gap.c
  scan/alphabet.c
  scan/keyword.c
  scan/identifier.c
  scan_test.c)
