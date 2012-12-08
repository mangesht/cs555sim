#!/bin/sh
gif=$(echo $1 | sed -e "s/\..*/.gif/g")
echo "#GNUplot script file for plotting x(t) with time "
echo "# x0"
echo "unset log"
echo "unset label"
echo "set autoscale"
echo 'set title "Mean number of customers"'
echo 'set xlabel "Time "'
echo 'set ylabel "Mean number of customers"'
echo "#set xrange [0:1]"
echo "set yrange [0:7.5]"
echo "#set boxwidth 0.00005"
echo "set terminal gif"
echo 'set output ' '"'$gif'"'
echo "plot " '"'$1'"' "using 1:2 title 'Number of Customers' with lines "
 

