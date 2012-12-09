#include<stdio.h>
#include<math.h>
#include<string.h>
#include "simLibAdv.h"
#include "eventLib.h"

#define NUM_SERVERS 2
#define BUFFER_SIZE 5
#define MAX_SIM_RUNS 30 
#define NUM_OBSEVATIONS 50000
#define MAX_CUST (NUM_SERVERS + BUFFER_SIZE)

LinkedList el; // This is event list sorted  
Server *server[NUM_SERVERS];
InputStream *inStr;
PacketQ *pktBuffer;
double curTime;
int debug=0;
long seed1 = -1;
long seed2 = -1;
long seed3 = -1;
double arrivalRate = 2.0;
double serviceRate = 1.0;
int initialNumCust = 0 ; 
int bufferSize = 5;
double simEndTime = 10.0;
double simEndTimeArg = 10.0;
double statCollectInterval = 0.05;
int statCollectIdx = 0;
int simIdx = 0 ; 
int numCustomers[NUM_OBSEVATIONS];
double avgNumCustomers[NUM_OBSEVATIONS]; 
int windowSize = 5;
long num_iterations = 20;
double sIdleTime[NUM_SERVERS];
double sIdlingStartTime[NUM_SERVERS];
int mode = 0;
double stationaryStateTime = 0;
double *blockingProb;
double *meanTimeCustSpend;
double *meanNumCustomers;
int numArrivals=0;
int numBlocked=0;
int numAllowedCust = 0;
double totTimeSpent = 0 ;
double *numCustHist;

char *outFileName;

void initialize();
void processEvent(Event *ep);
void startServicing();
void processArrival(Event *ep);
void processServiced(Event *ep);
void reset();
void calc_mean_avg_cust();
void filter_lpf(int windowSize);
void print_to_file(char*);
void reset_stats();
void calc_stat_param();
void solve_problem();
double calc_s2n(double *x,double xbn);
double calc_xbn(double *x);

void collectStat() {
    Event *ecstat;
    int num_cust=0;
    int i;
    if(mode == 0 || (mode == 1 && curTime > stationaryStateTime)){
       // Collect the statistical information
       num_cust = pktBuffer->curSize;
       for(i=0;i<NUM_SERVERS;i++){
           if(server[i]->status == BUSY) num_cust++;
       }
       if(mode == 0 ) { 
           avgNumCustomers[statCollectIdx] += num_cust;    
            numCustomers[statCollectIdx] = num_cust;
       }
       numCustHist[num_cust]++;
       statCollectIdx++;
    }
    // Add new stat collect event 
    ecstat = new Event();
    ecstat->schedTime = curTime + statCollectInterval ; 
    ecstat->eventType = COLLECT_STAT;
    el.insert_sort(ecstat);
    if(debug) printf("Added data collection event %lf \n",ecstat->schedTime);

}

void display_help(){
   printf("\n Usage : sim [-s1 seed] [-s2 seed] [-s3 seed][-lambda lambda] [-u mu] [-ic initialNumCust] [-et simEndTime]\n");
   printf("[-w windowSize] [-itr num_iterations] [-h] [-sci statCollectInterval\n");
   printf("[-mode modeval] [-sts stationary_stateTime] \n");
   printf("modeval 0 : For Finding out stationary region \n\t 1 : For find stationary region parameters\n");
   printf("\nseed : This value is used as seed for random number generator. s1 seed is used for input stream random number generator.\ns2 and s3 seeds are used for server 1 and server 2 random generator respectively.");
  printf("\nlambda : This value maps to arrival rate of customers in the system.\n");
  printf("mu     : This value maps to service rate of the servers in the system.\n");
  printf("initialNumCust : This represents intial number of customers in the system\n");
  printf("simEndTime     : This time is used to find out end of simulation. When the current simulation exceeds this time, the simulator stops\n");
  printf("windowSize     : Windowsize used for calculation of moving window averages \n");
  printf("num_iterations : The number of iterations for which an simulation needs to be run\n");
  printf("statCollectInterval : The system parameters like mean number of customers are sampled at an interval represented by this parameter\n"); 
  printf("modeval       : This parameter decides the mode of simulator.\n 0:\t The simulation is run for finding out warming up period for the system.\n 1:\t This simulation is run for finding out stationary state system parameters like mean number of customers, blocking probability , etc. \n There is no difference in these modes as far running the simulation is concerned. The mode just affects the stats collection functions.\n");
 printf("-h \t: Shows the help for the simulator\n");
}
int main(int argc,char *argv[]){

    Event *ep;

    int i;
    long itr;
    char *p;
    int agcCount = 1;
    int numSim;
    
   
    // Process command line inputs 
    p = (char *) malloc(256); 
    while(agcCount < argc){
        strcpy(p,argv[agcCount]);
        if(strstr(p,"-s1") != NULL){
            if(argc > agcCount+1) {
                seed1 = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
         if(strstr(p,"-s2")!=NULL){
            if(argc > agcCount+1) {
                seed2 = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-s3")!=NULL){
            if(argc > agcCount+1) {
                seed3 = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-lam")!=NULL){
            if(argc > agcCount+1) {
                arrivalRate = ator(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-u")!=NULL){
            if(argc > agcCount+1) {
               serviceRate  = ator(argv[agcCount+1]);
               agcCount++;
            }
        }
        if(strstr(p,"-et")!=NULL){
            if(argc > agcCount+1) {
               simEndTimeArg  = ator(argv[agcCount+1]);
               agcCount++;
            }
        }
        if(strstr(p,"-ic")!=NULL){
            if(argc > agcCount+1) {
                initialNumCust = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-w")!=NULL){
            if(argc > agcCount+1) {
                windowSize = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-itr")!=NULL){
            if(argc > agcCount+1) {
                num_iterations = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-h")!=NULL){
            display_help();
            return 0;
        }
        if(strstr(p,"-sci")!=NULL){
            if(argc > agcCount+1) {
               statCollectInterval  = ator(argv[agcCount+1]);
               agcCount++;
            }
        }
        if(strstr(p,"-mode")!=NULL){
            if(argc > agcCount+1) {
                mode = atoi(argv[agcCount+1]);
                agcCount++;
            }
        }
        if(strstr(p,"-sts")!=NULL){
            if(argc > agcCount+1) {
               stationaryStateTime  = ator(argv[agcCount+1]);
               agcCount++;
            }
        }


        agcCount++;
    }
    
    outFileName = (char *) malloc(256);
    strcpy(outFileName,"simrun");
    //pktBuffer = (Packet **) malloc(BUFFER_SIZE*sizeof(Packet *)) 
    pktBuffer = new PacketQ(bufferSize); // size of the pktBuffer
    inStr = new InputStream(seed1);
    for(i=0;i<NUM_SERVERS;i++){
        if(i==0) server[i] = new Server(seed2);
        if(i==1) server[i] = new Server(seed3);
        //server[i].rng->randomize_seed(-1); // Mangesh Fix this
    }
    
    blockingProb = (double *) malloc(num_iterations*sizeof(double));
    meanTimeCustSpend = (double *) malloc(num_iterations*sizeof(double));
    meanNumCustomers = (double *) malloc(num_iterations*sizeof(double));
    numCustHist = (double *) malloc((MAX_CUST+1)*sizeof(double));
    reset();
    simEndTime = simEndTimeArg;
    for(numSim=0;numSim<num_iterations;numSim++) { 
            if(mode == 1) {
                simEndTime = simEndTimeArg * (numSim+1); 
            }
            printf("--------------------------------------------------------------\n"); 
            printf("Starting simulation  : \n");
            if(mode == 0) {
                printf("for finding stationary region\n");
            }else{
                printf("for finding statastical parameters in stationary region\n");
            }
            printf("Number of servers = %d Buffer Size = %d \n",NUM_SERVERS,bufferSize);
            printf("Arrival rate = %3.2lf customers/Second Service Rate = %3.2lf customers/second \n",arrivalRate,serviceRate);
            printf("Initial number of customers in system %d \n",initialNumCust);
            printf("Simulation End time = %0.2lf \n",simEndTime);
            if(mode == 0) {
            }else{
                printf("Station state achieved after %lf seconds\n",stationaryStateTime);
            }
            printf("--------------------------------------------------------------\n"); 
            
            if(mode == 0 || numSim == 0 ) {
                initialize();
            }else{
                reset_stats();
            }

            while(curTime < simEndTime) {
                ep = el.pop();
                curTime = ep->schedTime;
                if(debug) printf("Time : %lf \n",curTime);
                processEvent(ep);
                if(debug) printf("processEvent ended \n");
                free(ep);
                startServicing();
                if(debug) printf("Service Ended \n");
            } 
            
            for(i=0;i<NUM_SERVERS;i++){
                if(debug) printf("Server %d Idle time = %lf \n",i,sIdleTime[i]);
            }
            if(mode == 1) { 
                calc_stat_param();
                reset_stats();
            }
            simIdx++;
    }

    char *prefix;
    prefix = (char *) malloc(256); 
    if(mode == 0 ) {
        printf("Writing to file \n");
        calc_mean_avg_cust();
        filter_lpf(windowSize);
        sprintf(prefix,"i%d_w%d",initialNumCust,windowSize);
        print_to_file(prefix);
    }else if(mode ==1 ) { 
        printf("Iteration Blocking  Mean Time Customer      Mean Number of\n");
        printf(" Index      Prob      spend in System          Customers\n");
        for(i=0;i<simIdx;i++){
            printf("%d\t%lf \t%lf \t\t%lf \n",i+1,blockingProb[i],meanTimeCustSpend[i],meanNumCustomers[i]);
        }
        solve_problem();
    }
    printf("Total number of iterations = %d \n",simIdx);

    return 0;
}

void initialize(){
    int i;
    double ar;
    Event *ep;
    struct Packet *p;
    double serviceTime;
    int ret;
    inStr->setArrivalRate(arrivalRate);
    curTime = 0.0;
    statCollectIdx = 0 ; 
    printf("initialize started \n");
    for(i=0;i<NUM_SERVERS;i++){
        server[i]->setServiceRate(serviceRate);
        server[i]->status = IDLE;
        sIdlingStartTime[i] = curTime;
    }
    pktBuffer->clean();
    el.clean();
    // Initial number of customers in systems
    for(i=0;i<initialNumCust;i++){
         if(i<2){
                // These guys should go in server
                ep = new Event();
                serviceTime = server[i]->getServiceTime();
                ep->schedTime = curTime + serviceTime;
                ep->eventType = SERVICED;
                ep->serverId = i;
                p = (struct Packet *) malloc(sizeof(struct Packet));
                p->arrivalTime = curTime;
                p->serviceStartTime = curTime;
                el.insert_sort(ep);
                server[i]->servP = p;
                server[i]->status = BUSY;
                sIdleTime[i] += curTime - sIdlingStartTime[i];
                if(debug) printf("Service started for packet aTime = %lf by server = %d to be over by %lf\n",p->arrivalTime,i,ep->schedTime);

         }else{
               // Add this packet to buffer
              p = (struct Packet *) malloc(sizeof(struct Packet));
              p->arrivalTime = curTime;
              ret = pktBuffer->push(p);
              if(ret<0){
                   if(debug) printf("Packet is dropped,Buffer Full\n");
              }else{
                   if(debug) printf("Packet pushed to pktBuffer \n");
              }
         }
    }
    // Plan first arrival
    ar = inStr->getArrivalInterval();
    ep = new Event();
    ep->schedTime = curTime + ar ; 
    ep->eventType = ARRIVAL;
    el.insert_sort(ep);
    if(debug) printf("Added arrival for time %lf \n",ep->schedTime);
    collectStat();
}

void processEvent(Event *ep){
    if(debug) printf("Processing event %d \n",ep->eventType);
    switch(ep->eventType){
        case ARRIVAL : processArrival(ep);break;
        case SERVICED : processServiced(ep);break;
        case COLLECT_STAT : collectStat();break;
    }
}
void processArrival(Event *ep){
    int ret;
    double ar;
    Event *earp;
    // Push this inside the pktBuffer
    struct Packet *p;
    p = (struct Packet *) malloc(sizeof(struct Packet));
    p->arrivalTime = curTime;
    ret = pktBuffer->push(p);
    numArrivals++;
    if(ret<0){
        if(debug) printf("Packet is dropped,Buffer Full\n");
        numBlocked++;
    }else{
        if(debug) printf("Packet pushed to pktBuffer \n");
        numAllowedCust++;
    }

    // Plan next arrival 
    ar = inStr->getArrivalInterval();
    earp = new Event();
    earp->schedTime = curTime + ar ; 
    earp->eventType = ARRIVAL;
    el.insert_sort(earp);
    if(debug) printf("Added arrival for time %lf \n",earp->schedTime);
}

void processServiced(Event *ep){
    if(debug) printf("processServiced %d \n",ep->serverId);
    server[ep->serverId]->status = IDLE;
    sIdlingStartTime[ep->serverId] = curTime;
    totTimeSpent += curTime - server[ep->serverId]->servP->arrivalTime; 
    free(server[ep->serverId]->servP); 
    if(debug) printf("processServiced end\n");
    //server[ep->serverId]->servP.serviceFinishTime = curTime; // This can used for stats
}

void startServicing(){
    int i;
    int idx;
    double serviceTime;
    struct Packet *p;
    Event *ep;
    idx = random() % NUM_SERVERS; 
    if(debug) printf("startServicing \n");
    for(i=0;i<NUM_SERVERS;i++){
        if(server[idx]->status == IDLE){
            p = pktBuffer->pop();
            if(p==NULL) {
                // Buffer empty
                break;
            }else{
                // Start service for this Packet 
                ep = new Event();
                serviceTime = server[idx]->getServiceTime();
                ep->schedTime = curTime + serviceTime;
                ep->eventType = SERVICED;
                ep->serverId = idx;
                p->serviceStartTime = curTime;
                el.insert_sort(ep);
                server[idx]->servP = p;
                server[idx]->status = BUSY;
                sIdleTime[idx] += curTime - sIdlingStartTime[idx];
                if(debug) printf("Service started for packet aTime = %lf by server = %d to be over by %lf\n",p->arrivalTime,idx,ep->schedTime);
            }
        }
        idx++;
        if (idx == NUM_SERVERS) idx = 0 ; 
    }
}


void reset(){
    int i;
    int j;
    for(i=0;i<NUM_OBSEVATIONS;i++){
        avgNumCustomers[i] = 0.0;
    }
    for(i=0;i<NUM_OBSEVATIONS;i++){
        numCustomers[i] = 0;
    }
    for(i=0;i<num_iterations;i++){
        blockingProb[i] = 0 ;
        meanTimeCustSpend[i] = 0 ;
        meanNumCustomers[i] = 0 ;
    }
    for(i=0;i<=MAX_CUST;i++){
        numCustHist[i] = 0;
    }
}

void calc_mean_avg_cust(){
    int i ; 
    for(i=0;i<statCollectIdx;i++){
        avgNumCustomers[i] = avgNumCustomers[i] / simIdx;
    } 
}
void filter_lpf(int windowSize){
    int i;
    int j;
    double tsum;
    for(i=0;i<statCollectIdx-windowSize+1;i++){
       tsum = 0;
        for(j=0;j<windowSize;j++){
            tsum = tsum + avgNumCustomers[i+j];
        }
        avgNumCustomers[i] = tsum / windowSize; 
    }
}
void print_to_file(char *postFix){
    char * charbuf;
    char * tbuf;
    FILE *fp;
    char *fName;
    int i;
    charbuf = (char *) malloc(16*statCollectIdx);
    charbuf = (char *) malloc(16);
    fName = (char *) malloc(256);
    strcpy(fName,outFileName);
    if(postFix != NULL) 
        if(strlen(postFix) != 0)
            strcat(fName,postFix);
    strcat(fName,"avg.dat"); 
    fp = fopen(fName,"w");
    if(debug) printf("Printting data for %d \n",statCollectIdx);
    if(fp == NULL) {
        printf("ERROR: Could not open %s \n",fName);
    }
    for(i=0;i<statCollectIdx-windowSize;i++){
        fprintf(fp,"%lf %lf\n",statCollectInterval*i,avgNumCustomers[i]);
    }
    fclose(fp); 
}

void reset_stats(){
    int i;
    numBlocked = 0;
    numArrivals = 0 ; 
    totTimeSpent = 0 ; 
    numAllowedCust = 0 ;
    for(i=0;i<NUM_OBSEVATIONS;i++){
        numCustomers[i] = 0 ;
    }
    statCollectIdx = 0 ;
    for(i=0;i<=MAX_CUST;i++){
        numCustHist[i] = 0;
    }
}

void calc_stat_param(){

    int i;
    double tCust = 0;
    double meanTh=0;
    double tsum = 0;
    blockingProb[simIdx] = (double) numBlocked / (double) numArrivals ; 
    meanTimeCustSpend[simIdx] = totTimeSpent / (double) numAllowedCust ;
    //for(i=0;i<statCollectIdx;i++){
    //    tCust += numCustomers[i];
    //}
   // meanNumCustomers[simIdx] = tCust/statCollectIdx;

    for(i=0;i<=MAX_CUST;i++){
        tsum +=numCustHist[i];
    }
    for(i=0;i<=MAX_CUST;i++){
        meanTh += (i * numCustHist[i])/tsum;
    }
    meanNumCustomers[simIdx] = meanTh;
    //printf("The 2 means are %lf Th %lf \n",meanNumCustomers[simIdx],meanTh); 
    
}
void solve_problem(){
    double xbn;
    double s2n;
    double sqrt_s2n_o_n;
    double conf_level = 90 ;
    double alpha ;
    double z_alpha_b_2  = 1.645;
    double int_b2;

    xbn = calc_xbn(blockingProb);
    //printf("xbn = %lf \n",xbn);
    s2n = calc_s2n(blockingProb,xbn);
    //printf("s2n = %lf \n",s2n);
    sqrt_s2n_o_n = sqrt(s2n/simIdx);
    alpha = (100 - conf_level) / 100 ;
    int_b2 = z_alpha_b_2 * sqrt_s2n_o_n;
    printf("Mean blocking probability \nconfidence internal = %lf to %lf with %lf confidence level. mean = %lf  \n",xbn-int_b2,xbn+int_b2,conf_level,xbn);

    // For Mean time customers spend in system 
    xbn = calc_xbn(meanTimeCustSpend);
    //printf("xbn = %lf \n",xbn);
    s2n = calc_s2n(meanTimeCustSpend,xbn);
    //printf("s2n = %lf \n",s2n);
    sqrt_s2n_o_n = sqrt(s2n/simIdx);
    alpha = (100 - conf_level) / 100 ;
    int_b2 = z_alpha_b_2 * sqrt_s2n_o_n;
    printf("Mean Time customers spend in system \n confidence internal = %lf to %lf with %lf confidence level. mean = %lf  \n",xbn-int_b2,xbn+int_b2,conf_level,xbn);

    // For Mean number of customers in system 
    xbn = calc_xbn(meanNumCustomers);
    s2n = calc_s2n(meanNumCustomers,xbn);
    sqrt_s2n_o_n = sqrt(s2n/simIdx);
    alpha = (100 - conf_level) / 100 ;
    int_b2 = z_alpha_b_2 * sqrt_s2n_o_n;
    printf("Mean Number Of Customers in system \n confidence internal = %lf to %lf with %lf confidence level. mean = %lf  \n",xbn-int_b2,xbn+int_b2,conf_level,xbn);


}

double calc_xbn(double *x){
    double tsum;
    int i;
    tsum = 0;
    for(i = 0 ; i < simIdx;i++){
        tsum += x[i];
    }
    tsum = tsum / simIdx;
    return tsum;
}
double calc_s2n(double *x,double xbn){
    double tsum;
    int i;
    tsum = 0;
    printf("SimIdx = %lf \n",(double)simIdx);
    for(i = 0 ; i < simIdx;i++){
        tsum += pow((x[i] - xbn),2.0);
        //printf("tsum %lf pow  = %lf \n",tsum,pow((x[i] - xbn),2.0));
    }
    tsum = tsum / (double)(simIdx-1);
    return tsum;
}
