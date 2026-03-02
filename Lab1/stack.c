#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

struct stack* top = NULL;
void push(int value){
    struct stack* p;
    p = top;
    top = (struct stack*)malloc(sizeof(struct stack));
    top->value = value;
    top->next = p; 
}

void pop(){  
   struct stack* p;
   if (top != NULL){
   	p = top;
   	top = top->next;
   	free(p);
   }	
}
void print(){
	struct stack* t = top;
	while(t != NULL){
		printf("%d ", t->value);
		t=t->next;
	}
}
