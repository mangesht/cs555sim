#include<stdio.h>
#include<math.h>
#include<string.h>
#include "simLibAdv.h"
#include "eventLib.h"

#define NUM_SERVERS 2
#define BUFFER_SIZE 5

LinkedList el; // This is event list sorted  
Server server[NUM_SERVERS];
InputStream inStr(-1);
PacketQ *pktBuffer;
double curTime;

void initialize();
void processEvent(Event *ep);
void startServicing();
void processArrival(Event *ep);
void processServiced(Event *ep);
void collectStat(Event *ep) {
}

int main(){
    Event *ep;

    int i;
    long itr;
    long num_iterations = 10;
   
    //pktBuffer = (Packet **) malloc(BUFFER_SIZE*sizeof(Packet *)) 
    pktBuffer = new PacketQ(10); // size of the pktBuffer
    for(i=0;i<NUM_SERVERS;i++){
        server[i].rng->randomize_seed(-1); // Mangesh Fix this
    }
   
    initialize();

    for(itr=0;itr<num_iterations;itr++){
        ep = el.pop();
        curTime = ep->schedTime;
        printf("Time : %lf \n",curTime);
        processEvent(ep);
        free(ep);
        startServicing();
    } 
    return 0;
}

void initialize(){
    int i;
    double ar;
    Event *ep;
    inStr.setArrivalRate(5);
    curTime = 0.0;

    for(i=0;i<NUM_SERVERS;i++){
        server[i].setServiceRate(8);
    }
    // Plan first arrival
    ar = inStr.getArrivalInterval();
    ep = new Event();
    ep->schedTime = curTime + ar ; 
    ep->eventType = ARRIVAL;
    el.insert_sort(ep);
    printf("Added arrival for time %lf \n",ep->schedTime);
}

void processEvent(Event *ep){
    printf("Processing event %d \n",ep->eventType);
    switch(ep->eventType){
        case ARRIVAL : processArrival(ep);break;
        case SERVICED : processServiced(ep);break;
        case COLLECT_STAT : collectStat(ep);break;
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
    ar = inStr.getArrivalInterval();
    earp = new Event();
    earp->schedTime = curTime + ar ; 
    earp->eventType = ARRIVAL;
    el.insert_sort(earp);
    printf("Added arrival for time %lf \n",earp->schedTime);
}

void processServiced(Event *ep){
    server[ep->serverId] = IDLE;
    //server[ep->serverId]->servP.serviceFinishTime = curTime; // This can used for stats
}

void startServicing(){
    int i;
    int idx;
    double serviceTime;
    struct Packet *p;
    Event *ep;
    idx = random() % NUM_SERVERS; 
    for(i=0;i<NUM_SERVERS;i++){
        if(server[idx].status == IDLE){
            p = pktBuffer->pop();
            if(p==NULL) {
                // Buffer empty
                break;
            }else{
                // Start service for this Packet 
                ep = new Event();
                serviceTime = server[idx].getServiceTime();
                ep->schedTime = curTime + serviceTime;
                ep->eventType = SERVICED;
                p->serviceStartTime = curTime;
                el.insert_sort(ep);
                server[idx].servP = p;
                server[idx] = BUSY;
                printf("Service started for packet aTime = %lf by server = %d to be over by %lf\n",p->arrivalTime,idx,ep->schedTime);
            }
        }
        idx++;
        if (idx == NUM_SERVERS) idx = 0 ; 
    }
}
