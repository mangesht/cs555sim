#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eventLib.h"

int main(){
    PacketQ pq(10);
    struct Packet *p;
    struct Packet *q;
    int i;
    int ret;
    for(i=0;i<200;i++){
        if((random()%100)<70){
            p = (struct Packet *) malloc(sizeof(struct Packet));
            p->arrivalTime = i;
            ret=pq.push(p);
            if(ret<0){
                printf("Push failed size = %d \n",pq.curSize);
            }else{
                printf("Pushed %d \n",i);
            }
        }
        if((random()%100)<30){
            q = pq.pop();
            if(q==NULL){
                printf("Pop failed size = %d \n",pq.curSize);
            }else{
                printf("Popped %lf \n",q->arrivalTime);
            }
        }
    }
    pq.displayAll();
    return 0;
}
