#include <stdio.h>
#include <stdlib.h>
#include "lab.h"
#include <pthread.h>
stack* top = NULL;
pthread_mutex_t lock;

void pushsafe(int value) {
    pthread_mutex_lock(&lock);
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->value = value;
    temp->next = top;
    top = temp;
    pthread_mutex_unlock(&lock);
}

void* thrcreate(void* arg) {
    int start_val = *((int*)arg);
    for (int i = start_val; i <= 1000000; i += 2) {
        pushsafe(i);
        printf("push: %d\n", i);
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

    pthread_create(&t1,NULL,thrcreate,&s1);
    pthread_create(&t2,NULL,thrcreate,&s2);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);


    //display();
    countel();
    return 0;

}
