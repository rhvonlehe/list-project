#include "string.h"
#include "dlist.h"


void dlist_init(dlist_t* list_p, int16_t element_offset, bool (*func)(void*, void*))
{
    list_p->first_p = 0;
    list_p->last_p = 0;
    list_p->element_offset = element_offset;
    list_p->count = 0;
    list_p->compare_func = func;
}

void dlist_append(dlist_t *list_p, void *node_p)
{
    dlist_element_t* element_p;

    // Get the element pointer for the new item
    //
    element_p = (dlist_element_t*) ((uint8_t*) node_p + list_p->element_offset);

    // New element is at end, points to nothing
    //
    element_p->next_p = 0;

    if (list_p->first_p)
    {
        element_p->prev_p = list_p->last_p;
        (list_p->last_p)->next_p = element_p;
        list_p->last_p = element_p;
    }
    else
    {
        element_p->prev_p = 0;
        list_p->first_p = element_p;
        list_p->last_p = element_p;
    }

    list_p->count++;
}

int16_t dlist_get_count(dlist_t* list_p)
{
    return list_p->count;
}

void* dlist_get_at(dlist_t *list_p, int16_t index)
{
    int count = 0;
    void* retval = 0;
    dlist_element_t *element_p;

    if (list_p->first_p)
    {
        element_p = list_p->first_p;

        while (count < index)
        {
            if (0 == element_p->next_p)
            {
                return 0; // nothing at that index
            }

            count++;
            element_p = element_p->next_p;
        }

        retval = (void*)((uint8_t*) element_p - list_p->element_offset);
    }

    return retval;
}

static void dlist_remove(dlist_t* list_p, void* node_p)
{
    dlist_element_t* element_p;

    element_p = (dlist_element_t*) ((uint8_t*) node_p + list_p->element_offset);

    list_p->count--;

    if (element_p == list_p->first_p)
    {
        if (element_p == list_p->last_p)
        {
            list_p->first_p = 0;
            list_p->last_p = 0;
        }
        else
        {
            (element_p->next_p)->prev_p = 0;
            list_p->first_p = element_p->next_p;
        }
    }
    else if (element_p == list_p->last_p)
    {
        list_p->last_p = element_p->prev_p;
        (list_p->last_p)->next_p = 0;
    }
    else
    {
        (element_p->prev_p)->next_p = element_p->next_p;
        (element_p->next_p)->prev_p = element_p->prev_p;
    }

    element_p->next_p = 0;
    element_p->prev_p = 0;
}

void dlist_remove_at(dlist_t *list_p, int16_t index)
{
    void* node_p = dlist_get_at(list_p, index);

    dlist_remove(list_p, node_p);
}

void* dlist_find(dlist_t *list_p, void* value_p)
{
    // traverse and call function pointer on each element to compare against value_p
    //
    dlist_element_t* element_p;
    void* node_p;

    if (list_p->first_p)
    {
        element_p = list_p->first_p;

        do
        {
            node_p = (void*)((uint8_t*) element_p - list_p->element_offset);
            if (list_p->compare_func(value_p, node_p))
            {
                return node_p;
            }
        } while ((element_p = element_p->next_p)); // while next_p exists

    }

    return 0;
}
