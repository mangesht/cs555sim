./sim -lambda 2.0 -u 1.0 -ic 0 -et 50 -w 5 -itr 1000 -sci 0.05 -s1 12345 -s2 34567 -s3 78901 >a
../scripts/plot_avg.sh simruni0_w5avg.dat > ../scripts/simruni0_w5.p
gnuplot ../scripts/simruni0_w5.p

./sim -lambda 2.0 -u 1.0 -ic 7 -et 50 -w 5 -itr 1000 -sci 0.05   -s1 12345 -s2 34567 -s3 78901 >a
../scripts/plot_avg.sh simruni7_w5avg.dat > ../scripts/simruni7_w5.p
gnuplot ../scripts/simruni7_w5.p

./sim -lambda 2.0 -u 1.0 -ic 7 -et 50 -w 15 -itr 1000 -sci 0.05   -s1 12345 -s2 34567 -s3 78901 >a
../scripts/plot_avg.sh simruni7_w15avg.dat > ../scripts/simruni7_w15.p
gnuplot ../scripts/simruni7_w15.p
