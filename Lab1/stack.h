#ifndef STACK_H
#define STACK_H

struct stack{
	int value;
	struct stack* next;
};

void push(int value);
void pop();
void printall();
#endif 
