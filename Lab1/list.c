#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct list* head = NULL;
struct list* tail = NULL;
void add(int value){
    struct list* l = (struct list*)malloc(sizeof(struct list));
    if(l == NULL) return;	
    l->value = value;
    l->next = NULL;
    if(tail != NULL) tail->next = l;
    else head = l;
    tail = l;    	
}

void pop_front(){
	if(head == NULL) return;
	struct list* temp = head;
	head = head->next;
	if(head == NULL) tail = NULL;
	free(temp);
}

void pop_back(){
	if(head == NULL) return;
	if(tail == head){
		head = NULL;
		return;
	}
	struct list* temp = head;
	while(temp->next->next != NULL){
		temp = temp->next;
	}
	temp->next = NULL;
	tail = temp;
}
void print(){
	struct list* temp = head;
	while(temp != NULL){
		printf("%d ", temp->value);
		temp = temp->next;
	}
}
