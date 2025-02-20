#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tree
{
    int key;
    struct tree *left, *right;
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

void inorder(tree *root)
{
    if (root == NULL)
        return;
    if (root->left)
        inorder(root->left);
    printf("%d ", root->key);
    if (root->right)
        inorder(root->right);
}

void btUpRight(tree *root)
{
    if (root == NULL)
        return;

    printf("%d ", root->key);

    if (root->right)
        btUpRight(root->right);
}

void btUpLeft(tree *root)
{
    if (root == NULL)
        return;

    if (root->left)
        btUpLeft(root->left);
        
    printf("%d ", root->key);
}

void btUpView(tree *root)
{
    if(root == NULL)
        return;

    btUpLeft(root->left);
    printf("%d ", root->key);
    btUpRight(root->right);
}

int main(void)
{
    int n;

    tree *tr = NULL;
    // scanf("%d", n);
    // tr->key = n;

    while (scanf("%d", &n) == 1)
    {
        if (n == 0)
        {
            break;
        }

        insert(&tr, n);
    }

    btUpView(tr);

    return 0;
}