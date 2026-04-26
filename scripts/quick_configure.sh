#!/bin/bash

dir_="generated"

exec cmake -B $dir_ \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           -DCMAKE_SKIP_BUILD_RPATH=TRUE \
           "$@"
