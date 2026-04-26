#!/bin/bash

prog_=$1

if [ $# -lt 1 -o ! -e $prog_ ]; then
  echo "Usage: $0 <program> [arguments...]" >&2
  exit 1
fi

dir_="generated"

export LD_LIBRARY_PATH=$dir_

shift 1

exec $prog_ "$@"
