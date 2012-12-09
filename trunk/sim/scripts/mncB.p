#GNUplot script file for plotting x(t) with time 
# x0
unset log
unset label
set autoscale
set title "Mean number of customers"
set xlabel "Iterations "
set ylabel "Mean number of customers"
set xrange [0:600]
#set yrange [0:7.5]
#set boxwidth 0.00005
set terminal gif
set output "mncB.gif"
plot "mncB.dat" using 1:2:3 title 'Number of Customers' with yerrorbars 
 

