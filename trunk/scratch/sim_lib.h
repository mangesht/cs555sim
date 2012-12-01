#include<time.h>
#include<math.h>
void set_seed(int seed) {
    char *state;
    char *ret;
    state = (char *) malloc(512);
    ret = (char *) malloc(512);
    ret = initstate(seed,state,64);
}
double rand_d() {
    long r_l;
    double d_div;
    r_l = random();
    d_div = (double) r_l / RAND_MAX ;
    //printf("d_div = %lf ",d_div);
    return d_div;
}

