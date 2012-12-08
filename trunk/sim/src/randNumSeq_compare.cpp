#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include <iostream>
#include <vector>
#include "simLibAdv.h"

int compare4qsort (const void * a, const void * b);
void findCommon (double A[], double B[], int iter);

void Usage(){
   printf("\nUsage : randNumSeq_compare  [maxitems_in_seq] \n");
   printf("maxitems_in_seq (Default value 10) : This parameter denotes how many random number needs to be generated per sequence \n");
}

int main(int argc,char *argv[]){
    Usage();
    int num_seq=2;
    double r_d;
    unsigned int seed=0;
    time_t cur_t, cur_lat_t;
    clock_t start;
    struct tm *loc_t;
    int num_divisions=1000;
    int maxitems_in_seq=10;
    if (argc > 1){
        maxitems_in_seq = atoi(argv[1]);
    }
    double r_unit ;
    RandomGen rng(seed);

    double *rnd_seq1 = (double *)malloc(maxitems_in_seq*sizeof(double));;
    double *rnd_seq2 = (double *)malloc(maxitems_in_seq*sizeof(double));;
    // double rnd_seq1[maxitems_in_seq];
    // double rnd_seq2[maxitems_in_seq]; //TODO  can be convered to a 2D array instead of two 1D arrays ?

    
    for (int j=0; j < num_seq; j++ ) {

	// Seed based on current time on every run
    	cur_t = time(NULL);
    	loc_t = localtime(&cur_t);
	printf ("\n#########################################\n");
    	printf("Current Time hh %d MM %d SS %d gmtoff = %ld \n",loc_t->tm_hour,loc_t->tm_min,loc_t->tm_sec,loc_t->tm_gmtoff);
    	seed = (loc_t->tm_hour+1)*(loc_t->tm_min+1)*(loc_t->tm_sec+1)*(loc_t->tm_year);
        printf("Seed for Sequence %d = %d \n",j+1,seed);
	printf ("#########################################\n");
    	rng.randomize_seed(seed);
    	r_d = rng.rand_d();
    	printf("Sample Rand Number = %lf \n",r_d);
   
	// Generate and store random numbers	
	for(int i=0;i<maxitems_in_seq;i++) {
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
    	for(int i=0;i<maxitems_in_seq;i++){
        	printf("%f \n",rnd_seq1[i]);
        	//printf("%1.3f \n",rnd_seq1[i]);
    	}

      	printf("Sequence 2 \n");
        for(int i=0;i<maxitems_in_seq;i++){
                printf("%f \n",rnd_seq2[i]);
        }

	qsort (rnd_seq1, maxitems_in_seq, sizeof(double), compare4qsort);
	qsort (rnd_seq2, maxitems_in_seq, sizeof(double), compare4qsort);

	printf("\nSequence 1 (Sorted)\n");
        for(int i=0;i<maxitems_in_seq;i++){
                printf("%f \n",rnd_seq1[i]);
                //printf("%1.3f \n",rnd_seq1[i]);
        }

        printf("Sequence 2 (Sorted)\n");
        for(int i=0;i<maxitems_in_seq;i++){
                printf("%f \n",rnd_seq2[i]);
        }

	//http://www.leetcode.com/2010/03/here-is-phone-screening-question-from.html

	findCommon(rnd_seq1,rnd_seq2,maxitems_in_seq);



    return 0;
}



int compare4qsort (const void * a, const void * b)
{
  //    return ( *(int*)a - *(int*)b );
        const double *da = (const double *) a;
        const double *db = (const double *) b;
        return (*da > *db) - (*da < *db);
}


void  findCommon(double A[], double B[], int iter) {
  	int i = 0, j = 0;
        int count =0;
  	while (i < iter && j < iter) {
    		if (A[i] > B[j]) {
        		printf ( "%f > %f",A[i],B[j]);
			printf("\ta is greater\n");
      			j++;
    		} else if (B[j] > A[i]) {
        		printf ( "%f < %f",A[i],B[j]);
			printf("\tb is greater\n");
      			i++;
    		} else {
			printf("\t Found common %d\n", A[i]);
      			i++;
      			j++;
			count++;
    		}
  	}

	if (count == 0) {

		 printf("\t ##########################################################################################\n");
		 printf("\t There are no common items found  between the two generated sequence each of length %d\n",count,iter);
		 printf("\t ##########################################################################################\n");
	} else {

		 printf("\t ######################################################################################\n");
                 printf("\t There are %d common items found between the two generated sequence each of length %d\n",count,iter);
                 printf("\t ######################################################################################\n");
		
	}
	
}
