#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct tree
{
    int32_t key;
    struct tree *left, *right;
    struct tree *parent; // ссылка на родителя
} tree;

void insert(tree **root, int key)
{
    if (!(*root))
    {
        *root = calloc(1, sizeof(tree));
        (*root)->key = key;
    }
    else if (key < (*root)->key)
        insert(&((*root)->left), key);
    else
        insert(&((*root)->right), key);
}

tree *search_tree_i(tree *root, int key)
{
    tree *find = root;
    while (find && find->key != key)
    {
        if (key < find->key)
            find = find->left;
        else
            find = find->right;
    }
    return find;
}

tree *findBrother(tree *root, int key)
{
    tree *elem = search_tree_i(root, key);

    if(elem == NULL || elem->parent == NULL || elem->key != key) {
        return 0;
    }
    
    root = elem->parent;

    if (root->left)
    {
        if (root->left->key != key)
        {
            return root->left;
        }
    }

    if (root->right)
    {
        if (root->right->key != key)
        {
            return root->right;
        }
    }

    return 0;
}

int main(void)
{
    int n;
    int key;

    tree *tr = NULL;

    while (scanf("%d", &n) == 1)
    {
        if (n == 0)
        {
            break;
        }

        insert(&tr, n);
    }

    scanf("%d", &key);

    tree *bro = findBrother(tr, key);

    printf("%d", bro->key);

    return 0;
}