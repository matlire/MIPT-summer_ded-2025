#!/bin/bash

launch='false'

while getopts ":l" arg; do
  case $arg in
    l) launch='true' ;;
  esac
done

make clean
make all

if [[ "$launch" == "true" ]]; then
    clear
    ./quadratic/dist/solver.out
fi
