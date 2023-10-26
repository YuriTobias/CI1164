#!/usr/bin/gnuplot -c
## set encoding iso_8859_15
set encoding utf
set terminal qt persist
set grid
set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
set xrange ["64":]
set xlabel  "Array size"

# Gerando figura PNG
set terminal png
set ylabel  ARG1
set output ARG2
plot ARG3 title ARG4 with linespoints, \
     ARG5 title ARG6 with linespoints, \
     ARG7 title ARG8 with linespoints
replot
unset output

