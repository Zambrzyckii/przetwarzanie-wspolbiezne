#include <stdio.h>
#include <stdlib.h>
#include "lab.h"
#include <pthread.h>
#include <semaphore.h>
stack* top = NULL;
sem_t semaphore;
void pushsafe(int value) {
    sem_wait(&semaphore);
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->value = value;
    temp->next = top;
    top = temp;
    sem_post(&semaphore);
}

void* thrcreate(void* arg) {
    int start_val = *((int*)arg);
    for (int i = start_val; i <= 100; i += 2) {
        pushsafe(i);
        printf("push: %d\n", i);
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
    sem_wait(&semaphore);
    stack* temp = top;
    int counter = 0;
    if (temp == NULL) return;
    while (temp != NULL) {
        counter++;
        temp = temp->next;
    }
    printf("\ncount: %d\n", counter);
    sem_post(&semaphore);
}
int main() {
    pthread_t t1,t2;
    int s1 = 1;
    int s2 = 0;

    sem_init(&semaphore, 0, 1);
    pthread_create(&t1,NULL,thrcreate,&s1);
    pthread_create(&t2,NULL,thrcreate,&s2);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);


    //display();
    countel();
    sem_destroy(&semaphore);
    return 0;

}
