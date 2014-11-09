#include <stdio.h>
#include <stdbool.h>
#include "dlist.h"

const unsigned int DLIST_TEST_ITEMS = 10;

typedef struct
{
    uint32_t data;

    dlist_element_t dlist_node;
} my_element_t;

static bool my_element_compare(void* a_p, void* b_p)
{
    if (*((uint32_t*) a_p) == *((uint32_t*) b_p))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool test_dlist(void)
{
    unsigned int i;
    bool retval = true;
    static dlist_t my_list;
    static my_element_t my_elements[10];

    // Initialize a list to be 10 elements with the data items set to index^2
    //
    dlist_init(&my_list, COMPUTE_OFFSET(my_element_t, dlist_node), my_element_compare);

    for (i = 0; i < DLIST_TEST_ITEMS; i++)
    {
        my_elements[i].data = i*i;
        dlist_append(&my_list, &my_elements[i]);
    }

    // Test that all items got appended (dlist_get_count), and their values using
    // are correct (dlist_get_at)
    //
    if (DLIST_TEST_ITEMS == (unsigned int) dlist_get_count(&my_list))
    {
        for (i = 0; i < DLIST_TEST_ITEMS; i++)
        {
            uint32_t* val_p = ((uint32_t*)(dlist_get_at(&my_list, i)));

            if (0 == val_p)
            {
                retval = false;
                break;
            }
            printf("i^2 is %d\n", i*i);
            printf("*val_p is %d\n", *val_p);
            if ((i*i) != *val_p)
            {
                retval = false;
                break;
            }
        }
    }
    else
    {
        retval = false;
    }


    // Remove an item from index 5, check that new index 5 value is old index 6 value
    //
    dlist_remove_at(&my_list, 5);

    uint32_t* val_p = ((uint32_t*) (dlist_get_at(&my_list, 5)));
    printf("New value at index 5 is %d\n", *val_p);
    if (6*6 != *val_p)
    {
        retval = false;
    }

    // Find the value 49 (7^2)
    //
    int compare_val = 49;
    my_element_t* node_p;
    node_p = dlist_find(&my_list, (void*)&compare_val);
    if (!node_p)
    {
        retval = false;
    }
    printf("value of node is %d\n", *((uint32_t*)node_p));


    // Don't find the value 25 (5^2)
    //
    compare_val = 25;
    if (dlist_find(&my_list, (void*)&compare_val))
    {
        retval = false;
    }

    return retval;
}




int main(void)
{
    if (test_dlist())
    {
        printf("dlist test passed\n");
    }
    else
    {
        printf("dlist test failed\n");
    }

    return 0;
}

