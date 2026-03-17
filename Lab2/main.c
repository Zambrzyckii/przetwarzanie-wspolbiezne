#include <stdio.h>
#include <stdlib.h>
#include "lab.h"
#include <pthread.h>
stack* top = NULL;
pthread_mutex_t lock;
pthread_cond_t cond;
int currentexp = 0;
void pushsafe(int value) {
   // pthread_mutex_lock(&lock);
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->value = value;
    temp->next = top;
    top = temp;
   // pthread_mutex_unlock(&lock);
}

void* thrcreate(void* arg) {
    int start_val = *((int*)arg);
    const char* tag = start_val ? "ODD" : "EVEN";
    for (int i = start_val; i <= 10000; i += 2) {
        pthread_mutex_lock(&lock);
        while(currentexp != i) pthread_cond_wait(&cond,&lock);
        pushsafe(i);
        printf("%s: %d\n",tag, i);
        currentexp++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void display() {
    pthread_mutex_lock(&lock);
    stack* temp = top;
    if (temp == NULL) return;

    while (temp != NULL) {
        printf("%d ", temp->value);
        temp = temp->next;
    }
    pthread_mutex_unlock(&lock);
}

void countel() {
    pthread_mutex_lock(&lock);
    stack* temp = top;
    int counter = 0;
    if (temp == NULL) return;
    while (temp != NULL) {
        counter++;
        temp = temp->next;
    }
    printf("\ncount: %d\n", counter);

    pthread_mutex_unlock(&lock);
}
int main() {
    pthread_t t1,t2;
    int s1 = 1;
    int s2 = 0;

    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);

    pthread_create(&t1,NULL,thrcreate,&s1);
    pthread_create(&t2,NULL,thrcreate,&s2);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);


    //display();
    countel();
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;

}
