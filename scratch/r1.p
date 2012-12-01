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
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set terminal gif
set output "r1_out.gif"
plot "r1.out" using 1:xtic(2)
 

