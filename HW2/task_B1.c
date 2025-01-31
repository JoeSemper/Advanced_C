#include <stdio.h>
#include <stdint.h>

typedef struct list
{
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

size_t totalMemoryUsage(list *head)
{
    if (head == NULL)
    {
        return 0;
    }

    static unsigned int total = 0;

    total += (*head).size;

    if (head->next != NULL)
    {
        totalMemoryUsage(head->next);
    }

    return total;
}