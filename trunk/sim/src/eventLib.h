#include<stdio.h>
#include<math.h>
#include<stdlib.h>

class Event {
    public :
    double schedTime;
    Event *next;
    Event *prev;
    Event(){
        schedTime = 0 ; 
        next = NULL;
        prev = NULL;
    }  
};
class LinkedList {
private :
    Event *start;
    Event *cur;
    Event *tail;

    int insert_after(Event *pos,Event *item);
    int insert_before(Event *pos,Event *item);
public :
    int insert_sort(Event *item);
    Event *pop();
    void displayAll();

    LinkedList(){
        start = NULL;
        cur   = NULL;
        tail  = NULL;
    }
};

int LinkedList::insert_sort(Event *item){
    if (item == NULL) {
        return -1;
    }
    // If the list is empty add at first position and initialize pointers

    if(start == NULL) {
        printf("Adding first item\n");
        start = item;
        tail = item;
        cur = item;
    }else{
        // Search the list from last position
        // Find appropriate location for the new event and place it there
        printf("Inserting \n");
        if(cur->schedTime > item->schedTime) {
            // Search in backward direction
            printf("In backward direction \n");
            while( (cur != start) && (cur->schedTime > item->schedTime)){
                    cur = cur->prev;
            }
            if((cur == start) && cur->schedTime > item->schedTime){
                insert_before(start,item);
            }else{
                insert_after(cur,item);
            }
        }else{
            // Search in forward direction
            printf("In forward direction \n");
            while( (cur != tail) && (cur->schedTime < item->schedTime)){
                cur = cur->next;
            }
            if((cur == tail) && cur->schedTime < item->schedTime){
                printf("Calling after \n");
                insert_after(tail,item);
            }else{
                printf("Calling before \n");
                insert_before(cur,item);
            }
        }
    }
    printf("Insertion done \n");
    return 0;
}

Event * LinkedList::pop(){
    Event *ret;
    ret = NULL;
    ret = start;
    if(start!= NULL){ 
        if(cur == start)
            cur = start->next;
        if(start == tail)
            tail = NULL;
        start->prev = NULL;
        start = start->next;
    }
    return ret;
}


int LinkedList::insert_after(Event *pos,Event *item){
    
    item->prev = pos;
    item->next = pos->next;
    if(pos == tail){
        tail = item;
    }else{
        pos->next->prev = item;
    }
    pos->next = item;
    
    return 1;    
}

int LinkedList::insert_before(Event *pos,Event *item){
    item->next = pos;
    item->prev = pos->prev;
    if(start != NULL) { 
        printf("Start = %lf tail = %lf pos = %lf item = %lf \n",start->schedTime,tail->schedTime,pos->schedTime,item->schedTime);
    }
    if(pos == start) {
        start = item;
    }else{
        pos->prev->next = item;
    }
    pos->prev  = item;
    return 1;
}
void LinkedList::displayAll(){
    Event *head;
    head = start;
    printf("The linked list is \n");
    while(head != NULL) {
        printf("%lf -> ",head->schedTime);
        head = head->next;
    }
    printf(" NULL \n");
}
