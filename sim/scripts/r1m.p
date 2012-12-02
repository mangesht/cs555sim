#GNUplot script file for plotting x(t) for different integrations steps
# x0
unset log
unset label
set autoscale
set title "Distribution of random number generator"
set xlabel "Value range"
set ylabel "Frequency"
set xrange [0:1]
set yrange [0:1800]
set boxwidth 0.0005
set terminal gif
set output "r1m_out.gif"
plot "r1m.out" using 1:2 title 'Distribution' with boxes 
 

