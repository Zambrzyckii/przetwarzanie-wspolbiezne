#include <stdio.h>
#include <stdlib.h>
#include "lab.h"

stack* head = NULL;
/*stack *create(int value) {
    stack *newstack = (stack*)malloc(sizeof(stack));
    newstack->value = value;
    newstack->next = NULL;
    head = newstack;
    return newstack;
}*/

void push(stack** temstack, int value) {
    if (temstack == NULL) {
        stack* temp = (stack*)malloc(sizeof(stack));
        temp->value = value;
        temp->next = NULL;
        *temstack = temp;
        head = temp;
        return;
    }
    stack* temp = (stack*)malloc(sizeof(stack));
    temp->value = value;
    temp->next = *temstack;
    *temstack = temp;
    head = temp;
    return;
}
void pop(stack *temstack) {
    if (head == NULL) return;
    stack* mktemp = head;
    head = head->next;
    temstack = temstack->next;
    free(mktemp);
}
