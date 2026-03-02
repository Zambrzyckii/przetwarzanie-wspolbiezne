#ifndef LIST_H
#define LIST_H

struct list{
	int value;
	struct list* next;
};

void add(int value);
void pop_front();
void pop_back();
void print();
#endif
