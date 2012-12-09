#GNUplot script file for plotting x(t) with time 
# x0
unset log
unset label
set autoscale
set title "Blocking Probability"
set xlabel "Iterations"
set ylabel "Probability"
set xrange [0:600]
#set yrange [0:7.5]
#set boxwidth 0.00005
set terminal gif
set output "bpA.gif"
plot "bpA.dat" using 1:2:3 title 'Blocking Prob' with yerrorbars 
 

