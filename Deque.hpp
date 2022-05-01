#ifndef CODE_HEADER_H
#define CODE_HEADER_H

#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <typeinfo>
#include <limits.h>


//MACRO

#define Deque_DEFINE(t)															\
																				\
struct Deque_##t##_Iterator; /* forward decl */									\
void Deque_##t##_Iterator_ctor(Deque_##t##_Iterator * it, size_t idx, 			\
							size_t at_element, t **data, size_t capacity,		\
									size_t * head, size_t * tail);				\
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it, 						\
													Deque_##t##_Iterator end);	\
																				\
/* define DEQUE */																\
 																				\
const char t##_type_name[] = "Deque_" #t;										\
																				\
struct Deque_##t {																\
	t *data;																	\
	char type_name[sizeof(t##_type_name)]; 										\
	size_t head;																\
	size_t tail;																\
	size_t sz; 																	\
	size_t capacity;															\
	size_t (*size)(Deque_##t *de); 												\
	bool (*empty)(Deque_##t *de);												\
	void (*push_back)(Deque_##t *de, t item);									\
	void (*push_front)(Deque_##t *de, t item);									\
	t& (*back)(Deque_##t *de); 													\
	t& (*front)(Deque_##t *de);													\
	t& (*at)(Deque_##t *de, size_t idx);										\
	bool (*pop_back)(Deque_##t *de);											\
	bool (*pop_front)(Deque_##t *de);											\
	void (*clear)(Deque_##t *de);												\
	void (*dtor)(Deque_##t *de);												\
	bool (*comparator)(const t& o1, const t &o2);								\
	Deque_##t##_Iterator (*begin)(Deque_##t *de);								\
	Deque_##t##_Iterator (*end)(Deque_##t *de);									\
	void (*sort)(Deque_##t *de, Deque_##t##_Iterator begin, 					\
												Deque_##t##_Iterator end);		\
};																				\
																				\
	/* Iterator struct-------------------------------------------------*/		\
struct Deque_##t##_Iterator {													\
	size_t idx; 																\
	size_t at_element;															\
	size_t capacity;															\
	void (*inc)(Deque_##t##_Iterator *it);										\
	void (*dec)(Deque_##t##_Iterator *it);										\
	t& (*deref)(Deque_##t##_Iterator *it);										\
	t ** data; 																	\
	size_t * head; 																\
	size_t * tail;																\
};																				\
																				\
/* DEQ member fns------------------------------------------------ */			\
size_t Deque_##t##_size(Deque_##t *de) {										\
	return de->sz;																\
}																				\
																				\
bool Deque_##t##_empty(Deque_##t *de) {											\
	if(!de->sz) return true;													\
	return false;																\
}																				\
																				\
void Deque_##t##_push_back(Deque_##t *de, t item) {								\
	if(de->sz < de->capacity){													\
		de->data[de->tail] = item;												\
		de->tail = (de->tail < (de->capacity-1)) ? de->tail + 1 : 0; 			\
	}																			\
	else{																		\
		/* resize */															\
		size_t new_capacity = de->capacity*2; 									\
		t* new_data = (t*)malloc(sizeof(t)*new_capacity);						\
		Deque_##t##_Iterator b;													\
		Deque_##t##_Iterator e = de->end(de);									\
		size_t idx = 0; 														\
		for(b = de->begin(de); !Deque_##t##_Iterator_equal(b, e); b.inc(&b)) {	\
			new_data[idx] = b.deref(&b);										\
			idx++;																\
		}																		\
		free(de->data);															\
		de->data = new_data;													\
																				\
		de->capacity = new_capacity;											\
		de->head = new_capacity-1;												\
		de->tail = idx;															\
																				\
		de->data[de->tail] = item;												\
		de->tail = (de->tail < (de->capacity-1)) ? de->tail + 1 : 0;			\
	}																			\
	de->sz++;																	\
}																				\
																				\
void Deque_##t##_push_front(Deque_##t *de, t item) {							\
	if(de->sz < de->capacity){													\
		de->data[de->head] = item;												\
		de->head = (de->head > 0) ? de->head - 1 : de->capacity - 1;			\
	}																			\
	else{																		\
		/* rezize */															\
		size_t new_capacity = de->capacity*2; 									\
		t* new_data = (t*)malloc(sizeof(t)*new_capacity);						\
		Deque_##t##_Iterator b;													\
		Deque_##t##_Iterator e = de->end(de);									\
		size_t idx = 0; 														\
		for(b = de->begin(de); !Deque_##t##_Iterator_equal(b, e); b.inc(&b)) {	\
			new_data[idx] = b.deref(&b);										\
			idx++;																\
		}																		\
		free(de->data);															\
		de->data = new_data;													\
																				\
		de->capacity = new_capacity;											\
		de->head = new_capacity-1;												\
		de->tail = idx;															\
																				\
		de->data[de->head] = item;												\
		de->head = (de->head > 0) ? de->head - 1 : de->capacity - 1;			\
	}																			\
	de->sz++;																	\
}																				\
																				\
t& Deque_##t##_back(Deque_##t *de) {											\
	size_t idx = (de->tail > 0) ? de->tail - 1 : de->capacity - 1;				\
	return de->data[idx];														\
}																				\
																				\
t& Deque_##t##_front(Deque_##t *de){											\
	size_t idx = (de->head < (de->capacity - 1)) ? de->head + 1 : 0; 			\
	return de->data[idx];														\
}																				\
																				\
t& Deque_##t##_at(Deque_##t *de, size_t idx) {									\
	size_t idx_of_elem = (de->head + idx+ 1) % de->capacity; 					\
	return de->data[idx_of_elem];												\
}																				\
																				\
bool Deque_##t##_pop_back(Deque_##t *de){										\
	if(!de->empty(de)){ 														\
		de->tail = (de->tail > 0) ? de->tail - 1 : de->capacity - 1;			\
		de->sz--;																\
		return true;															\
	}																			\
	return false;																\
}																				\
																				\
bool Deque_##t##_pop_front(Deque_##t *de){										\
	if(!de->empty(de)){															\
		de->head = (de->head < (de->capacity -1)) ? de->head + 1 : 0;			\
		de->sz--;																\
		return true;															\
	}																			\
	return false;																\
}																				\
																				\
void Deque_##t##_clear(Deque_##t *de){ 											\
	de->head = 0;																\
	de->tail = 0;																\
	de->sz = 0;																	\
}																				\
																				\
void Deque_##t##_dtor(Deque_##t *de){											\
	free(de->data); 															\
	de->data = NULL;															\
}																				\
																				\
Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *de){							\
	Deque_##t##_Iterator it;													\
	size_t idx = (de->head < (de->capacity -1)) ? de->head + 1 : 0; 			\
	size_t at = (size_t)0;														\
	Deque_##t##_Iterator_ctor(&it, idx, at, &de->data, de->capacity, &de->head,	\
																	 &de->tail);\
	return it;																	\
}																				\
																				\
Deque_##t##_Iterator Deque_##t##_end(Deque_##t *de){							\
	Deque_##t##_Iterator it;													\
	size_t idx = de->tail; 														\
	size_t at = (size_t)de->sz;													\
	Deque_##t##_Iterator_ctor(&it, idx, at, &de->data, de->capacity, &de->head, \
																	&de->tail);	\
	return it;																	\
}																				\
																				\
int Deque_##t##_compare_for_sort(const void* o1, const void* o2, void *thunkIn){\
	Deque_##t *de = (Deque_##t*) thunkIn;										\
	t o1_cpy = *((t*)o1);														\
	t o2_cpy = *((t*)o2);														\
	bool less = de->comparator(o1_cpy, o2_cpy);									\
	bool greater = de->comparator(o2_cpy, o1_cpy);								\
	if(less){																	\
		return -1;																\
	}																			\
	else if(greater){															\
		return 1;																\
	}																			\
	else{																		\
		return 0;																\
	}																			\
}																				\
																				\
void Deque_##t##_sort(Deque_##t *de, Deque_##t##_Iterator begin,				\
												Deque_##t##_Iterator end){		\
	int ordered = 1;															\
	Deque_##t##_Iterator be = begin;											\
	Deque_##t##_Iterator en = end;												\
	Deque_##t##_Iterator next = begin;											\
	next.inc(&next);															\
	int val;																	\
	while(!Deque_##t##_Iterator_equal(next, en)){								\
		val = Deque_##t##_compare_for_sort(&be.deref(&be), &next.deref(&next), de);	\
		if(val == 1){															\
			ordered = 0;														\
			break;																\
		}																		\
		be.inc(&be);															\
		next.inc(&next);														\
	}																			\
	if(!ordered){																\
		t * tmp = (t*)malloc(sizeof(t)*de->capacity);							\
		size_t idx = 0;															\
		auto actual_begin = de->begin(de);										\
		auto actual_end = de->end(de);											\
																				\
		while(!Deque_##t##_Iterator_equal(actual_begin, actual_end)){			\
			tmp[idx] = actual_begin.deref(&actual_begin);						\
			idx++;																\
			actual_begin.inc(&actual_begin);									\
		}																		\
		size_t size = ((size_t)end.at_element) - ((size_t)begin.at_element);	\
		qsort_r(tmp+begin.at_element, size, sizeof(t), Deque_##t##_compare_for_sort, de);	\
		free(de->data);															\
		de->data = tmp;															\
		de->head = de->capacity-1;												\
		de->tail = de->sz;														\
	}																			\
}																				\
																				\
																				\
/* Iterator member fns------------------------------------------------ */		\
void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *it){						\
	it->idx = (it->idx < (it->capacity - 1)) ? it->idx + 1 : 0;					\
	it->at_element++;															\
}																				\
																				\
																				\
																				\
void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *it){						\
	it->idx = (it->idx > 0) ? it->idx - 1 : it->capacity - 1;					\
	it->at_element--;															\
}																				\
																				\
																				\
t& Deque_##t##_Iterator_deref(Deque_##t##_Iterator *it){							\
	size_t idx = ((*it->head) < (it->capacity - 1)) ? (*it->head) + 1 : 0; 		\
	idx += it->at_element;														\
	idx = idx % it->capacity;													\
	return (*it->data)[idx];													\
}																				\
																				\
/* ctor and other fns ------------------------------------------s----- */		\
bool Deque_##t##_equal(Deque_##t& de1, Deque_##t& de2){							\
	if(de1.sz != de2.sz) return false;											\
	Deque_##t##_Iterator it1, it2; 												\
	it1 = de1.begin(&de1);														\
	it2 = de2.begin(&de2);														\
	for(size_t i = 0; i < de1.sz; i++){ 										\
		bool less = de1.comparator(it1.deref(&it1), it2.deref(&it2));			\
		bool greater = de1.comparator(it2.deref(&it2), it1.deref(&it1));		\
		if(less || greater) return false;										\
		it1.inc(&it1);															\
		it2.inc(&it2);															\
	}																			\
	return true;																\
}																				\
																				\
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it, 						\
													Deque_##t##_Iterator end){	\
	return (it.idx == end.idx) && (it.at_element == end.at_element);			\
}																				\
																				\
void Deque_##t##_Iterator_ctor(Deque_##t##_Iterator * it, size_t idx, 			\
							size_t at_element, t **data, size_t capacity, 		\
							size_t* head, size_t* tail){						\
	it->idx = idx;																\
	it->at_element = at_element;												\
	it->inc = Deque_##t##_Iterator_inc;											\
	it->dec = Deque_##t##_Iterator_dec;											\
	it->deref = Deque_##t##_Iterator_deref;										\
	it->data = data;															\
	it->capacity = capacity;													\
	it->head = head;															\
	it->tail = tail;															\
}																				\
																				\
void Deque_##t##_ctor(Deque_##t *de, bool (*cmp)(const t &o1, const t &o2)){	\
	de->data = (t*)malloc(sizeof(t)*4);											\
	strcpy(de->type_name, t##_type_name);										\
	de->head = 4-1;																\
	de->tail = 0;																\
	de->sz = 0;																	\
	de->capacity = 4;															\
	de->size = Deque_##t##_size;												\
	de->empty = Deque_##t##_empty;												\
	de->push_back = Deque_##t##_push_back;										\
	de->push_front = Deque_##t##_push_front;									\
	de->back = Deque_##t##_back;												\
	de->front	= Deque_##t##_front;											\
	de->at = Deque_##t##_at;													\
	de->pop_back = Deque_##t##_pop_back;										\
	de->pop_front	= Deque_##t##_pop_front;									\
	de->clear	= Deque_##t##_clear;											\
	de->dtor = Deque_##t##_dtor;												\
	de->comparator = cmp;														\
	de->begin = Deque_##t##_begin;												\
	de->end = Deque_##t##_end;													\
	de->sort = Deque_##t##_sort;												\
}																				















#endif
