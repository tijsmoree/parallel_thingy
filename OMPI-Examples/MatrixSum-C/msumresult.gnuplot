set grid
set key
#set logscale y
set title "Speedup for summation of 2 matrices"
set xtics 
set ytics
#set ytics (1,10,100,1000,10000,100000)
set xlabel "Nr of processes"
set ylabel "Run Time (s)"
set output
set terminal X11
set yrange [12:17]
plot 'msum_N600.dat'  using 1:($2* 16) title "N=600 (x16)" with linespoints linewidth 4, \
     'msum_N1200.dat' using 1:($2*  4) title "N=1200 (x4)" with linespoints linewidth 4, \
     'msum_N2400.dat' using 1:($2*  1) title "N=2400 (x1)" with linespoints linewidth 4
set terminal postscript eps color solid "Helvetica" 36
set size 2.0,2.0
set output 'msumresult.eps'
replot
