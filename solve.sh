#!/usr/bin/bash

ARGS=$1

EXECDIR="bin"
mkdir -p $EXECDIR
make $ARGS > /dev/null 2>&1

for exe in $(find "$EXECDIR" -maxdepth 1 -type f -executable | sort); do
  echo "### $exe: "
  "$exe"
  echo ""
done

make clean > /dev/null 2>&1
