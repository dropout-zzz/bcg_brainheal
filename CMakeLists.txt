cmake_minimum_required(VERSION 4.2.0)

project(Bcg LANGUAGES C)

add_library(bcg_scanner SHARED
  scan/gap.c
  scan/alphabet.c
  scan/keyword.c
  scan/identifier.c
  scan/token.c)
target_compile_options(bcg_scanner PRIVATE
  -fvisibility=hidden)

add_library(bcg_diagnosis SHARED
  diagnosis.c)
target_compile_options(bcg_diagnosis PRIVATE
  -fvisibility=hidden)

add_executable(scan_test
  scan_test.c)
target_link_libraries(scan_test
  bcg_scanner
  bcg_diagnosis)

add_library(bcg_parser SHARED
  parse.c)
target_compile_options(bcg_parser PRIVATE
  -fvisibility=hidden)

add_executable(parse_test
  parse_test.c)
target_link_libraries(parse_test
  bcg_scanner
  bcg_parser
  bcg_diagnosis)

add_library(compiler OBJECT
  compile.c)

add_library(cgen OBJECT
  codegen.c)

add_executable(bcg
  bcg.c)
target_link_libraries(bcg
  bcg_diagnosis
  bcg_scanner
  bcg_parser
  compiler
  cgen)
