#!/bin/bash

dir_="generated"

exec cmake --build $dir_ \
           "$@"
