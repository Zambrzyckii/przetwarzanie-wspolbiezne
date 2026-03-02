#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main(){
    add(10);
    add(20);
    add(30);
    add(40);
    pop_front();
    print();
    printf("\n");
    pop_front();
    pop_back();
    print();
	return 0;
}
