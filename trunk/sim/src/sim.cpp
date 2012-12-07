#include<stdio.h>
#include<math.h>
#include<string.h>
#include "simLibAdv.h"
#include "eventLib.h"

#define NUM_SERVERS 2
#define BUFFER_SIZE 5
#define MAX_SIM_RUNS 30 
#define NUM_OBSEVATIONS 50000

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
double statCollectInterval = 0.05;
int statCollectIdx = 0;
int simIdx = 0 ; 
int numCustomers[MAX_SIM_RUNS][NUM_OBSEVATIONS];
double avgNumCustomers[NUM_OBSEVATIONS]; 
char *outFileName;

void initialize();
void processEvent(Event *ep);
void startServicing();
void processArrival(Event *ep);
void processServiced(Event *ep);
void reset();
void calc_mean();
void print_to_file();
void collectStat() {
    Event *ecstat;
    int num_cust=0;
    int i;
    // Collect the statistical information
    num_cust = pktBuffer->curSize;
    for(i=0;i<NUM_SERVERS;i++){
        if(server[i]->status == BUSY) num_cust++;
    }
    avgNumCustomers[statCollectIdx] += num_cust;    
    statCollectIdx++;
    // Add new stat collect event 
    ecstat = new Event();
    ecstat->schedTime = curTime + statCollectInterval ; 
    ecstat->eventType = COLLECT_STAT;
    el.insert_sort(ecstat);
    printf("Added arrival for time %lf \n",ecstat->schedTime);

}

void display_help(){
   printf("\n Usage : sim [-s1 seed] [-s2 seed] [-s3 seed][-lambda lambda] [-u mu] [-ic initialNumCust] [-et simEndTime\n");
}
int main(int argc,char *argv[]){

    Event *ep;

    int i;
    long itr;
    long num_iterations = 10;
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
               simEndTime  = ator(argv[agcCount+1]);
               agcCount++;
            }
        }
        if(strstr(p,"-ic")!=NULL){
            if(argc > agcCount+1) {
                initialNumCust = atoi(argv[agcCount+1]);
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
    
    reset();
    for(numSim=0;numSim<10;numSim++) { 

    printf("--------------------------------------------------------------\n"); 
    printf("Starting simulation with : \n");
    printf("Number of servers = %d Buffer Size = %d \n",NUM_SERVERS,bufferSize);
    printf("Arrival rate = %3.2lf customers/Second Service Rate = %3.2lf customers/second \n",arrivalRate,serviceRate);
    printf("Initial number of customers in system %d \n",initialNumCust);
    printf("Simulation End time = %0.2lf \n",simEndTime);
    printf("--------------------------------------------------------------\n"); 
    initialize();

    while(curTime < simEndTime) {
        ep = el.pop();
        curTime = ep->schedTime;
        printf("Time : %lf \n",curTime);
        processEvent(ep);
        if(debug) printf("processEvent ended \n");
        free(ep);
        startServicing();
        if(debug) printf("Service Ended \n");
    } 
    simIdx++;
    }

    calc_mean();
    print_to_file();
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
                printf("Service started for packet aTime = %lf by server = %d to be over by %lf\n",p->arrivalTime,i,ep->schedTime);

         }else{
               // Add this packet to buffer
              p = (struct Packet *) malloc(sizeof(struct Packet));
              p->arrivalTime = curTime;
              ret = pktBuffer->push(p);
              if(ret<0){
                   printf("Packet is dropped,Buffer Full\n");
              }else{
                   printf("Packet pushed to pktBuffer \n");
              }
         }
    }
    // Plan first arrival
    ar = inStr->getArrivalInterval();
    ep = new Event();
    ep->schedTime = curTime + ar ; 
    ep->eventType = ARRIVAL;
    el.insert_sort(ep);
    printf("Added arrival for time %lf \n",ep->schedTime);
    collectStat();
}

void processEvent(Event *ep){
    printf("Processing event %d \n",ep->eventType);
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
    if(ret<0){
        printf("Packet is dropped,Buffer Full\n");
    }else{
        printf("Packet pushed to pktBuffer \n");
    }

    // Plan next arrival 
    ar = inStr->getArrivalInterval();
    earp = new Event();
    earp->schedTime = curTime + ar ; 
    earp->eventType = ARRIVAL;
    el.insert_sort(earp);
    printf("Added arrival for time %lf \n",earp->schedTime);
}

void processServiced(Event *ep){
    if(debug) printf("processServiced %d \n",ep->serverId);
    server[ep->serverId]->status = IDLE;
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
                printf("Service started for packet aTime = %lf by server = %d to be over by %lf\n",p->arrivalTime,idx,ep->schedTime);
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
    for(j=0;j<MAX_SIM_RUNS;j++){
        for(i=0;i<NUM_OBSEVATIONS;i++){
            numCustomers[j][i] = 0;
        }
    }
}

void calc_mean(){
    int i ; 
    for(i=0;i<statCollectIdx;i++){
        avgNumCustomers[i] = avgNumCustomers[i] / simIdx;
    } 
}
void print_to_file(){
    char * charbuf;
    char * tbuf;
    FILE *fp;
    char *fName;
    int i;
    charbuf = (char *) malloc(16*statCollectIdx);
    charbuf = (char *) malloc(16);
    fName = (char *) malloc(256);
    strcpy(fName,outFileName);
    strcat(fName,"avg.dat"); 
    fp = fopen(fName,"w");
    printf("Printting data for %d \n",statCollectIdx);
    if(fp == NULL) {
        printf("Could not open %s \n",fName);
    }
    for(i=0;i<statCollectIdx;i++){
        fprintf(fp,"%lf %lf\n",statCollectInterval*i,avgNumCustomers[i]);
    }
    fclose(fp); 
}

