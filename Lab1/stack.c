#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

struct stack* head = NULL;

void push(int value){
     struct stack* NewStack = (struct stack*)malloc(sizeof(struct stack));
     NewStack-> value = value;
     if (head != NULL) NewStack->next = head;
     else{
        NewStack->next = NULL;
     }
    head = NewStack;
}

void pop(){
    if(head == NULL) return;
	struct stack* temp = head;
	head = head->next;
	free(temp);
}
void printall(){
	struct stack* temp = head;
	while(temp != NULL){
		printf("%d\n",temp->value);
		temp = temp->next;
	}
}
