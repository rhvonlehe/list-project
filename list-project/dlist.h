#ifndef DLIST_H
#define DLIST_H

#include <stdint.h>
#include <stdbool.h>

typedef struct dlist
{
    struct dlist* next_p;
    struct dlist* prev_p;
} dlist_element_t;

typedef struct
{
    dlist_element_t* first_p;
    dlist_element_t* last_p;

    int16_t count;
    int16_t element_offset;
    bool (*compare_func)(void*, void*);
} dlist_t;

#define COMPUTE_OFFSET(TYPE, ELEMENT)  ((int32_t)(&(((TYPE *)0)->ELEMENT)))

extern void dlist_init(dlist_t* list_p, int16_t element_offset, bool (*func)(void*, void*));

extern void dlist_append(dlist_t* list_p, void* node_p);

extern int16_t dlist_get_count(dlist_t* list_p);

extern void* dlist_get_at(dlist_t* list_p, int16_t index);

extern void dlist_remove_at(dlist_t* list_p, int16_t index);

extern void* dlist_find(dlist_t* list_p, void* value_p);


#endif // DLIST_H
