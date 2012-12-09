./sim -lambda 2.0 -u 1.0 -ic 0 -et 50 -w 5 -itr 10000 -sci 0.01 -mode 0 -s1 12345 -s2 34567 -s3 78901 >a
mv simruni0_w5avg.dat simruni0_w5avgA.dat 
../scripts/plot_avg.sh simruni0_w5avgA.dat > ../scripts/simruni0_w5A.p
gnuplot ../scripts/simruni0_w5A.p

./sim -lambda 2.0 -u 1.0 -ic 0 -et 50 -w 5 -itr 10000 -sci 0.01 -mode 0 -s1 890938 -s2 7098601 -s3 41908470 >a
mv simruni0_w5avg.dat simruni0_w5avgASeed.dat 
../scripts/plot_avg.sh simruni0_w5avgASeed.dat > ../scripts/simruni0_w5ASeed.p
gnuplot ../scripts/simruni0_w5ASeed.p


./sim -lambda 2.0 -u 1.0 -ic 7 -et 50 -w 50 -itr 10000 -sci 0.01  -mode 0  -s1 12345 -s2 34567 -s3 78901 >a
mv simruni7_w50avg.dat simruni7_w50avgA.dat
../scripts/plot_avg.sh simruni7_w50avgA.dat > ../scripts/simruni7_w50A.p
gnuplot ../scripts/simruni7_w50A.p

./sim -lambda 2.0 -u 1.0 -ic 7 -et 50 -w 15 -itr 10000 -sci 0.01  -mode 0  -s1 890938 -s2 7098601 -s3 41908470 >a
mv simruni7_w15avg.dat simruni7_w15avgA.dat
../scripts/plot_avg.sh simruni7_w15avgA.dat > ../scripts/simruni7_w15A.p
gnuplot ../scripts/simruni7_w15A.p

# System B 
./sim -lambda 10.0 -u 1.0 -ic 0 -et 10 -w 500 -itr 1000 -sci 0.001 -mode 0 -s1 12345 -s2 34567 -s3 78901 >a
mv simruni0_w500avg.dat simruni0_w500avgB.dat 
../scripts/plot_avg.sh simruni0_w500avgB.dat > ../scripts/simruni0_w500B.p
gnuplot ../scripts/simruni0_w500B.p

./sim -lambda 10.0 -u 1.0 -ic 4 -et 10 -w 500 -itr 1000 -sci 0.001  -mode 0  -s1 526589 -s2 1032598 -s3 7854689 >a
mv simruni4_w500avg.dat simruni4_w500avgB.dat
../scripts/plot_avg.sh simruni4_w500avgB.dat > ../scripts/simruni4_w500B.p
gnuplot ../scripts/simruni4_w500B.p

./sim -lambda 10.0 -u 1.0 -ic 4 -et 10 -w 15 -itr 1000 -sci 0.001  -mode 0  -s1 895626 -s2 2659842 -s3 2365598 >a
mv simruni4_w15avg.dat simruni4_w15avgB.dat
../scripts/plot_avg.sh simruni4_w15avgB.dat > ../scripts/simruni4_w15B.p
gnuplot ../scripts/simruni4_w15B.p


