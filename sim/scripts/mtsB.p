#GNUplot script file for plotting x(t) with time 
# x0
unset log
unset label
set autoscale
set title " Mean Time customers spend in system"
set xlabel "Number of Iterations "
set ylabel "Mean Time spent"
set xrange [0:600]
#set yrange [0:7.5]
#set boxwidth 0.00005
set terminal gif
set output "mtsB.gif"
plot "mtsB.dat" using 1:2:3 title 'Mean Time Spent' with yerrorbars 
 

