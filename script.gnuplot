#!/usr/bin/gnuplot
set autoscale
set format y "%s"
set title "Lagrange interpolation"
set key reverse Left outside
set grid
#set style data linespoints
plot "function.dat" title "Function", \
     "interpolation.dat" title "Interpolation"

exit
