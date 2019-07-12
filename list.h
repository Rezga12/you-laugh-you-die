#ifndef MEM_LIST_H
#define MEM_LIST_H

#include <stdbool.h>

struct list_elem{

	void * data;
	struct list_elem * next;
	struct list_elem * prev;
};

struct list{
	int size;
	struct list_elem * head;

};

void list_init(struct list * list);

void list_add(struct list * list,void * data);
int list_size(struct list * list);
void * list_remove(struct list * list, long long data, bool(*cmp)(long long, void*));
void * list_find(struct list * list, long long a, bool(*cmp)(long long, void*));

void list_free(struct list * list);




#endif