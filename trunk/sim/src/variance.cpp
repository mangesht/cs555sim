#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>

int main(int argc,char *argv[]){
	const int n=30;
    	//int samples[n] = {2,5,4,8,7,4,4,7,5,1,5,2,2,6,3,9,7,6,4,5, 6,0,4,8,2,1,4,4,1,5};
    	int samples[n] = {482,408,446,456,479,435,454,445,493,459,494,437,452,456,420,422,498,443,455,460,452,456,452,449,461,479,453,445,458,468};
	double variance=0;
	double sample_mean =0;

 	for(int i=0;i<n;i++){
                sample_mean = sample_mean + samples[i] ;
        }
	sample_mean = sample_mean/n ;

        printf("sample_mean  is %f \n", sample_mean);
	
        for(int i=0;i<n;i++){
		variance=variance + ((samples[i] - sample_mean)*(samples[i] - sample_mean));
        }

	variance=variance/(n-1);

        printf("Variance is %f \n", variance);

    return 0;
}

