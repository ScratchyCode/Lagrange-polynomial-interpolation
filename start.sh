#!/bin/bash
DIR=$(dirname "$0")
cd $DIR
gcc Lagrange.c tinyexpr.c -lm -o interpol.out
./interpol.out
gnuplot script.gnuplot -p

sleep 2
rm function.dat
rm interpolation.dat
rm interpol.out
exit
