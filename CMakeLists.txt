cmake_minimum_required(VERSION 4.2.0)

project(Bcg LANGUAGES C)

add_library(scanner OBJECT
  scan/gap.c
  scan/alphabet.c
  scan/keyword.c
  scan/identifier.c
  scan/token.c)
target_include_directories(scanner PUBLIC
  scan)

add_executable(scan_test
  scan_test.c)
target_link_libraries(scan_test
  scanner)
