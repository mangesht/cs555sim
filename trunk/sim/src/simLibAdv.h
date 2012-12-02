#include<time.h>
#include<math.h>
#include<stdlib.h>
#include "eventLib.h"
typedef enum {BUSY,IDLE} serverStatusType;

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

class InputStream{
    double arrival_rate;
    double time_period;
    RandomGen *rng;
    public:
    double getArrivalInterval();
    InputStream(int seed){
        rng = new RandomGen(-1);
        rng->randomize_seed(seed);
    }
    int setArrivalRate(double ar){
        arrival_rate = ar;
        time_period = 1 / ar ;
        return 0;
    }

};
class Server{
    double service_rate;
    double service_time;
    public:
    serverStatusType status;
    RandomGen *rng;
    Packet *servP; // Packet that is getting serviced 
    double getServiceTime();
    Server(int seed){
        rng = new RandomGen(-1);
        rng->randomize_seed(seed);
        status = IDLE;
    }
    Server(){
        rng = new RandomGen(-1);
        rng->randomize_seed(-1);
        status = IDLE;
    }
    int setServiceRate(double sr){
        service_rate = sr;
        service_time = 1 /sr;
        return 0;
    }
};
double InputStream::getArrivalInterval(){
    double ret;
    ret = rng->rand_d();
    ret = -1 * time_period * log(ret);
    return ret;
}
double Server::getServiceTime(){
    double ret;
    ret = rng->rand_d();
    ret = -1 * service_time * log(ret);
    return ret;
}

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
        seed = seed + random();
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

