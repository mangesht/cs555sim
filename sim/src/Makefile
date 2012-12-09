randAdv_test:randAdv_test.cpp simLibAdv.h
	g++ -o randAdv_test randAdv_test.cpp 

randNumSeq_compare: randNumSeq_compare.cpp simLibAdv.h
	g++ -o randNumSeq_compare randNumSeq_compare.cpp

sim:sim.cpp
	g++ -o sim sim.cpp -lm
 
unif_graph:randAdv_test
	./randAdv_test -d 1000 -i 10000000 > r10m.out
	gnuplot ../scripts/r10m.p
	./randAdv_test -d 1000 -i 1000000 > r1m.out
	gnuplot ../scripts/r1m.p

variance: variance.cpp
	g++ -o utils_variance variance.cpp 
clean: 
	rm -f *.out *.gif *.png core* *.dat ../scripts/sim*.p randAdv_test randNumSeq_compare sim utils_variance	
