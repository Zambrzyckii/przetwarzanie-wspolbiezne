#ifndef LAB_H
#define LAB_H

typedef struct stack {
    int value;
    struct stack *next;
}stack;

stack* create(int value);
void push(stack **stack, int value);
void pop(stack *stack);

#endif
