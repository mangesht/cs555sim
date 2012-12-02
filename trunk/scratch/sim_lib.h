#include<time.h>
#include<math.h>
class RandomGen{
    public:
    void set_seed(int seed);
    void randomize_seed(int seed=-1);
    double rand_d();
};
void set_seed(int seed) {
    char *state;
    char *ret;
    state = (char *) malloc(512);
    ret = (char *) malloc(512);
    ret = initstate(seed,state,64);
    printf("\nUsing seed = %d \n",seed);
}
void randomize_seed(int seed = -1){
    time_t cur_t;
    struct tm *loc_t;
    if(seed == -1) {
        cur_t = time(NULL);
        loc_t = localtime(&cur_t);
        printf("Cur Time hh %d MM %d SS %d gmtoff = %ld \n",loc_t->tm_hour,loc_t->tm_min,loc_t->tm_sec,loc_t->tm_gmtoff);
        seed = (loc_t->tm_hour+1)*(loc_t->tm_min+1)*(loc_t->tm_sec+1)*(loc_t->tm_year);
     }
     set_seed(seed);
}

double rand_d() {
    long r_l;
    double d_div;
    r_l = random();
    d_div = (double) r_l / RAND_MAX ;
    //printf("d_div = %lf ",d_div);
    return d_div;
}

