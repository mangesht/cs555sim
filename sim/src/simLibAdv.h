#include<time.h>
#include<math.h>
class RandomGen{
    int last_rand;
    struct random_data *rand_state;
    public:
    void set_seed(int seed);
    void randomize_seed(int seed=-1);
    double rand_d();
    RandomGen(int seed){
        last_rand = seed;
    }
};
void RandomGen::set_seed(int seed) {

    char *state;
    char *ret;
    state = (char *) malloc(512);
    ret = (char *) malloc(512);
/*
    ret = initstate(seed,state,64);
    printf("\nUsing seed = %d \n",seed);
    last_rand = seed;
*/
    rand_state = (struct random_data *) malloc(sizeof(struct random_data));
    int r= initstate_r(seed,state,64,rand_state);

}
void RandomGen::randomize_seed(int seed){
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

double RandomGen::rand_d() {
    double d_div;
//    srandom(last_rand);
//    last_rand = random();
    random_r(rand_state,&last_rand);
//    printf("r = %d ",last_rand);
    d_div = (double) last_rand / RAND_MAX ;
    //printf("d_div = %lf ",d_div);
    return d_div;
}

