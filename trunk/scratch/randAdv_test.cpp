#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include "simLibAdv.h"
int main(int argc,char *argv[]){
    double r_d;
    unsigned int seed;
    time_t cur_t;
    clock_t start;
    struct tm *loc_t;
    int num_divisions;
    int num_iterations;
    int *n_in_range;
    double r_unit ;
    int i;
    int j;
    RandomGen rng(seed);
    
    seed = 0 ; 
    if(argc > 1 ) { 
        printf("Coverting string to int %s ",argv[1]);
        seed = atoi(argv[1]);
    }else{
        cur_t = time(NULL);
        loc_t = localtime(&cur_t);
        printf("Cur Time hh %d MM %d SS %d gmtoff = %ld \n",loc_t->tm_hour,loc_t->tm_min,loc_t->tm_sec,loc_t->tm_gmtoff);
        seed = (loc_t->tm_hour+1)*(loc_t->tm_min+1)*(loc_t->tm_sec+1)*(loc_t->tm_year);
    }
    printf("Seed = %d \n",seed);
    //rng = new(seed);
    rng.randomize_seed(seed);
    r_d = rng.rand_d(); 
    printf("R = %lf ",r_d);
    
    num_divisions = 1000;
    num_iterations = 10*1000*1000;
    n_in_range = (int *) malloc(num_divisions * sizeof(int));
    for(i=0;i<num_divisions;i++){
        n_in_range[i] = 0;
    }
    r_unit =  1.0 / num_divisions ; 
    printf("r_unit = %lf \n",r_unit);
    for(i=0;i<num_iterations;i++) {
        r_d = rng.rand_d();
        for(j=0;j<num_divisions;j++){
            if(r_d < ((j+1) * r_unit ) ) {
                n_in_range[j]++;
                break;
            }
        }
    }

    double exp_mean;
    exp_mean = num_iterations / num_divisions;
    double s_d = 0 ; 
    for(j=0;j<num_divisions;j++){
        printf("%f %d\n",j*r_unit,n_in_range[j]);
        s_d = s_d + abs((exp_mean - n_in_range[j]));
    }
    s_d = s_d / num_divisions;
    //s_d = (sqrt(s_d) );
    
    printf("\nMean Absolute deviation / mean = %lf percent \n",(s_d*100)/exp_mean);
    return 0;
}
