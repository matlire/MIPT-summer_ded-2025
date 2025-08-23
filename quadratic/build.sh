rm -r build/*
gcc utils/*.h utils/*.c quadratic/*.h quadratic/*.c test/*.h test/*.c test/tests/*.c main.c -lm -o build/solver.out
./build/solver.out
