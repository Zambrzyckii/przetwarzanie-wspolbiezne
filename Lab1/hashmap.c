#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

hashmap* create(int size, int (*hash_func)(void*), int (*compare_func)(void*, void*)){
	hashmap* NewMap = (hashmap*)malloc(sizeof(hashmap));
	if(!NewMap) return NULL;
	NewMap->size = size;
	NewMap->hash_func = hash_func;
	NewMap->compare_func = compare_func;
	NewMap->option = (list**)calloc(size,sizeof(list*));
	return NewMap;
}

void add(hashmap* hashm, void* key, void* value){
	int idx = hashm->hash_func(key) % hashm->size;
	list* current = hashm->option[idx];
	while(current){
		if(hashm->compare_func(current->key,key) == 0){
			current->value = value;
			return;
		}
		current = current->next;
	}
	list* newlist = (list*)malloc(sizeof(list));
	newlist->key = key;
	newlist->value = value;
	newlist->next = hashm->option[idx];
	hashm->option[idx] = newlist;
}
void* get(hashmap* hmap, void* key){
	int idx = hmap->hash_func(key) % hmap->size;
	list* current = hmap->option[idx];
	while(current){
		if(hmap->compare_func(current->key,key) == 0) return current->value;
		current=current->next;
	}
	return NULL;
}
void removeel(hashmap* hmap, void* key){
	int idx = hmap->hash_func(key) % hmap->size;
	list* current = hmap->option[idx];
	list* previous = NULL;
	while(current){
		if(hmap->compare_func(current->key,key) == 0){
			if(previous) previous->next = current->next;
			else hmap->option[idx] = current->next;

			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

void destroy(hashmap* hmap){
	for(int i = 0; i < hmap->size;i++){
		list* current = hmap->option[i];
		while(current){
			list* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(hmap->option);
	free(hmap);
}
void printall(hashmap* hmap,void (*print_func)(void*, void*)){
   for(int i = 0; i < hmap->size;i++){
   	 list* current = hmap->option[i];
   	 if(current != NULL){
   	 	printf("Option %d ", i);
   	 	while(current){
   	 		print_func(current->key,current->value);
   	 		current = current->next;
   	 		if(current) printf("->");
   	 	}
   	 	printf("\n");
   	 }
   }
}
