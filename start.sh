#!/bin/bash
DIR=$(dirname "$0")
cd $DIR

gcc Lagrange.c tinyexpr.c -lm -o interpol.out -O2
./interpol.out

gnuplot script.gnuplot -p

rm -f interpol.out
rm -f funzione.dat
rm -f interpolazione.dat

exit
