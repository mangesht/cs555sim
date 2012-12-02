#include<stdio.h>
#include "event_list.cpp"
#include "sim_lib.h"

int main(int argc,char *argv[]){
    double scT;
    double lastSct;
    LinkedList el;
    int i;
    int seed = -1;
    Event *et;
    if(argc > 1 ) { 
        printf("Coverting string to int %s ",argv[1]);
        seed = atoi(argv[1]);
    }
    randomize_seed(seed);
    //el = new() ;
    lastSct = 0 ;  
    for(i=0;i<1000;i++){
        scT = rand_d() * 10 -5 ;
        scT = lastSct + scT;
        lastSct = scT;
        et = new Event;
        et->schedTime = scT;
        el.insert_sort(et);
        printf("ins Done \n");
        if(rand_d()*100<90){
            et = el.pop();
            free(et);
            printf(" pop \n");
        }
        el.displayAll();
    }
    printf("POP\n");
    et = el.pop();
    while(et!= NULL){
        printf("%lf -> ",et->schedTime);
        et = el.pop();
    } 
    printf("NULL\n");
    return 1;
}
