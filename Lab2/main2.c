#include <stdio.h>
#include <stdlib.h>
#include "lab.h"
#include <pthread.h>
#include <semaphore.h>
stack* top = NULL;
sem_t semaphore;
sem_t s1,s2;
void pushsafe(int value) {
   // sem_wait(&semaphore);
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->value = value;
    temp->next = top;
    top = temp;
    //sem_post(&semaphore);
}

void* thrcreate(void* arg) {
    int start_val = *((int*)arg);
    const char* tag = start_val ? "ODD" : "EVEN";
    for (int i = start_val; i <= 10000; i += 2) {
        if(start_val) sem_wait(&s1);
        else sem_wait(&s2);
        
        pushsafe(i);
        printf("%s: %d\n",tag, i);

        if(start_val) sem_post(&s2);
        else sem_post(&s1);
    }
    return NULL;
}
void display() {
    sem_wait(&semaphore);
    stack* temp = top;
    if (temp == NULL) return;

    while (temp != NULL) {
        printf("%d ", temp->value);
        temp = temp->next;
    }
   sem_post(&semaphore);
}


void countel() {
//    sem_wait(&semaphore);
    stack* temp = top;
    int counter = 0;
    if (temp == NULL) return;
    while (temp != NULL) {
        counter++;
        temp = temp->next;
    }
    printf("\ncount: %d\n", counter);
  //  sem_post(&semaphore);
}
int main() {
    pthread_t t1,t2;
    int st1 = 1;
    int st2 = 0;
                       
    sem_init(&s1, 0, 0);
    sem_init(&s2,0,1);
    
    pthread_create(&t1,NULL,thrcreate,&st1);
    pthread_create(&t2,NULL,thrcreate,&st2);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);


    //display();
    countel();
    sem_destroy(&s1);
    sem_destroy(&s2);
    return 0;

}
