randAdv_test:randAdv_test.cpp simLibAdv.h
	g++ -o randAdv_test randAdv_test.cpp 

sim:sim.cpp
	g++ -o sim sim.cpp -lm
 
unif_graph:randAdv_test
	./randAdv_test -d 1000 -i 10000000 > r10m.out
	gnuplot ../scripts/r10m.p
	./randAdv_test -d 1000 -i 1000000 > r1m.out
	gnuplot ../scripts/r1m.p

clean: 
	rm -f r10m.out r1m.out *.gif *.png randAdv_test sim	
