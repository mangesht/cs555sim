#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include "simLibAdv.h"

int main(int argc,char *argv[]){
    int num_seq=2;
    double r_d;
    unsigned int seed=0;
    time_t cur_t, cur_lat_t;
    clock_t start;
    struct tm *loc_t;
    int num_divisions=1000;
    int num_iterations=10;
    double r_unit ;
    RandomGen rng(seed);
    
    double rnd_seq1[num_iterations];
    double rnd_seq2[num_iterations]; //TODO

    
    for (int j=0; j < num_seq; j++ ) {

	// Seed based on current time on every run
    	cur_t = time(NULL);
    	loc_t = localtime(&cur_t);
	printf ("#########################################\n");
    	printf("Current Time hh %d MM %d SS %d gmtoff = %ld \n",loc_t->tm_hour,loc_t->tm_min,loc_t->tm_sec,loc_t->tm_gmtoff);
    	seed = (loc_t->tm_hour+1)*(loc_t->tm_min+1)*(loc_t->tm_sec+1)*(loc_t->tm_year);
        printf("Seed for Sequence %d = %d \n",j+1,seed);
	printf ("#########################################\n");
    	rng.randomize_seed(seed);
    	r_d = rng.rand_d();
    	printf("Sample Rand Number = %lf \n",r_d);
   
	// Generate and store random numbers	
	for(int i=0;i<num_iterations;i++) {
		// UGLY FIX ME
		if (j==0) {
        		rnd_seq1[i] = rng.rand_d();
		}
		else {
        		rnd_seq2[i] = rng.rand_d();
		}
    	}

	cur_lat_t = time(NULL);
	while (cur_lat_t == cur_t){
		cur_lat_t = time(NULL);
	}

    }


      	printf("\nSequence 1 \n");
    	for(int i=0;i<num_iterations;i++){
        	printf("%f \n",rnd_seq1[i]);
        	//printf("%1.3f \n",rnd_seq1[i]);
    	}

      	printf("Sequence 2 \n");
        for(int i=0;i<num_iterations;i++){
                printf("%f \n",rnd_seq2[i]);
        }

    
    return 0;
}
