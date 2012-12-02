#include <stdio.h>
#include <stdlib.h>
#include "simLibAdv.h"

int main() { 
    RandomGen r1(10);
    RandomGen r2(11);
    double d1;
    double d2;
    int i;
    r1.randomize_seed(10);
    r2.randomize_seed(11);
    for(i=0;i<10;i++){
        d1 = r1.rand_d();
        d2 = r2.rand_d();
        printf("%lf %lf \n",d1,d2);
    }
    

return 0;
}

