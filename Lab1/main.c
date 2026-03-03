#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int my_hash_func(void* key) {
    char* s = (char*)key;
    int hash = 0;
    while (*s) {
        hash = hash * 31 + *s++;
    }
    return abs(hash); 
}

int my_compare_func(void* key1, void* key2) {
    return strcmp((char*)key1, (char*)key2); 
}

void my_print_entry(void* key, void* value) {
    printf("{%s: %d}", (char*)key, *(int*)value);
}

int main() {
    int jan = 25;
    int anna = 30;
    int marek = 40;
    int nowy = 35;

    hashmap* hmap = create(5, my_hash_func, my_compare_func);
    if (!hmap) {
        return 1;
    }

    add(hmap, "Jan", &jan);
    add(hmap, "Anna", &anna);
    add(hmap, "Marek", &marek);


    add(hmap, "Jan", &nowy);

    printall(hmap, my_print_entry);
    printf("\n");
    removeel(hmap, "Anna");

    printall(hmap, my_print_entry);

    destroy(hmap); 

    return 0;
}
