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
target_include_directories(bcg_scanner PUBLIC
  scan)

add_library(bcg_diagnosis SHARED
  diagnosis.c)
target_compile_options(bcg_diagnosis PRIVATE
  -fvisibility=hidden)
target_link_libraries(bcg_diagnosis PRIVATE
  bcg_scanner)

add_executable(scan_test
  scan_test.c)
target_link_libraries(scan_test
  bcg_scanner
  bcg_diagnosis)

add_library(bcg_parser SHARED
  parse.c)
target_compile_options(bcg_parser PRIVATE
  -fvisibility=hidden)
target_link_libraries(bcg_parser PRIVATE
  bcg_scanner)

add_executable(parse_test
  parse_test.c)
target_link_libraries(parse_test
  bcg_scanner
  bcg_parser
  bcg_diagnosis)

add_library(compiler OBJECT
  compile.c)
target_link_libraries(compiler PRIVATE
  bcg_scanner)

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
