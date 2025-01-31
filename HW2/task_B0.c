#include <stdio.h>
#include <stdint.h>

typedef struct list
{
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

uint64_t findMaxBlock(list *head)
{
    if (head == NULL)
    {
        return 0;
    }

    static list max = {0, 0, 0, 0};

    if ((*head).size > max.size)
    {
        max = *head;
    }

    if (head->next != NULL)
    {
        findMaxBlock(head->next);
    }

    return max.address;
}