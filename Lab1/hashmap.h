#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct list {
   void* key;
   void* value;
   struct list* next;
	
}list;

typedef struct hashmap{
   struct list** option;
   int size;
   int (*hash_func)(void*);
   int (*compare_func)(void*,void*);
	
}hashmap;

hashmap* create(int size, int (*hash_func)(void*), int (*compare_func)(void*,void*));
void add(hashmap* map, void* key, void* value);
void* get(hashmap* map, void* key);
void removeel(hashmap* map,void* key);
void destroy(hashmap* map);
void printall(hashmap* map,void (*print_func)(void*,void*));

#endif
