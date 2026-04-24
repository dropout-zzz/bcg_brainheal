#!/bin/bash

dir_="generated"

exec cmake -B $dir_ \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           "$@"
