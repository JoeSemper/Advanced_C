#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list
{
    char word[20];
    struct list *next;
};

void add_to_list(char new_word[], struct list **head)
{
    struct list *new = calloc(1, sizeof(struct list));
    strcpy(new->word, new_word);

    if (*head == NULL)
    {
        *head = new;
    }
    else
    {
        struct list *p = *head;
        while (p->next != NULL)
            p = p->next;
        p->next = new;
    }
}

void swap_elements(struct list *p1, struct list *p2)
{
    char tmp[20] = {0};
    strcpy(tmp, p1->word);

    strcpy(p1->word, p2->word);
    strcpy(p2->word, tmp);
}

void print_list(struct list *head)
{
    while (head)
    {
        printf("%s ", head->word);
        head = head->next;
    }
    printf("\n");
}

void delete_list(struct list *head)
{
    struct list *tmp;

    while (head)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

struct list *read_strings(void)
{
    struct list *head = NULL;
    char c;
    char word[21] = {0};
    int i = 0;

    while ((c = getchar()) != '.')
    {
        if (c == ' ')
        {
            add_to_list(word, &head);
            for (int i = 0; i < 20; i++)
            {
                word[i] = '\0';
            }
            i = 0;
        }
        else
        {
            word[i] = c;
            i++;
        }
    }

    add_to_list(word, &head);

    return head;
}

void sort_list(struct list **head)
{
    int swapped;
    struct list *ptr1;
    struct list *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->word, ptr1->next->word) > 0)
            {
                swap_elements(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

int main(void)
{
    struct list *head = read_strings();

    sort_list(&head);

    print_list(head);
    
    delete_list(head);

    return 0;
}