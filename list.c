#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void l_free(struct list_elem * elem);

void list_init(struct list * list){
	list->size = 0;
	list->head = NULL;
}

void list_add(struct list * list,void * data){

	struct list_elem * elem = malloc(sizeof(struct list_elem));
	elem->data = data;
	elem->next = NULL;
	elem->prev = NULL;

	if(list->head == NULL){
		list->head = elem;
		return;
	}

	elem->next = list->head;
	list->head->prev = elem;
	list->head = elem;

	list->size++;

}


int list_size(struct list * list){
	return list->size;


}
void * list_remove(struct list * list, long long data, bool(*cmp)(long long, void*)){

	void * result = NULL;
	for(struct list_elem * ptr = list->head;ptr!=NULL;ptr=ptr->next){
		if(cmp(data,ptr->data)){
			list->size--;
			if(ptr->prev == NULL && ptr->next == NULL){
				list->head = NULL;
				result = ptr->data;
				free(ptr);
				return result;
			}
			if(ptr->prev == NULL){
				ptr->next->prev=NULL;
				list->head = ptr->next;
				result = ptr->data;
				free(ptr);
				return result;
			}
			ptr->prev->next = ptr->next;
			ptr->next->prev = ptr->prev;
			result = ptr->data;
			free(ptr);
			return result;

		}
	}
	return result;


}
void * list_find(struct list * list, long long data, bool(*cmp)(long long, void*)){

	for(struct list_elem * ptr = list->head;ptr!=NULL;ptr=ptr->next){
		if(cmp(data,ptr->data)){
			return ptr->data;
		}

	}
	return NULL;
}

void list_free(struct list * list){
	l_free(list->head);
}

void l_free(struct list_elem * elem){
	if(elem == NULL){
		return;
	}

	l_free(elem->next);
	free(elem->data);
	free(elem);
}