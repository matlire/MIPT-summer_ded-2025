rm -r build/*
gcc quadratic/quadratic.h quadratic/quadratic.c main.c -lm -o build/solver.out
./build/solver.out
