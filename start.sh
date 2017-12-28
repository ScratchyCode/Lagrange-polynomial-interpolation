#!/bin/bash
DIR=$(dirname "$0")
cd $DIR
./interpol.out
gnuplot script.gnuplot -p
sleep 2
rm function.dat
rm interpolation.dat
exit
