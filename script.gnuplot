#!/usr/bin/gnuplot
set autoscale
set format y "%s"
set title "Lagrange interpolation"
set key reverse Left outside
set grid
#set style data linespoints

plot "funzione.dat" w lines title "Analytical function", \
     "interpolazione.dat" w lines title "Lagrangian Interpolation"

exit
