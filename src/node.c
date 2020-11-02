#include "node.h"

struct node * n_alloc(char *s, int f)
{
    struct node *n = malloc(sizeof(struct node));

    n->data.s = calloc(sizeof(char), strlen(s) + 1);
    strcpy(n->data.s, s);
    n->data.f = f;
    n->left = NULL;
    n->right = NULL;

    return n;
}

void n_add_left(struct node *root, struct node *n)
{
    root->left = n;
}

void n_add_right(struct node *root, struct node *n)
{
    root->right = n;
}

void n_free(struct node *n)
{
    free(n->data.s);
    free(n);
}

void n_print(struct node *n)
{
    printf("%s - %d\n", n->data.s, n->data.f);
}

void n_sort(struct node **n, unsigned int len)
{
    bool sorted;
    unsigned int l = len;
    unsigned int i;

    do {
        sorted = true;

        for (i = 1; i < l; i++) {
            if (n[i - 1]->data.f > n[i]->data.f) {
                struct node *buff;

                buff = n[i - 1];
                n[i - 1] = n[i];
                n[i] = buff;

                sorted = false;
            }
        }

        l--;
    } while (!sorted);
}

struct node * n_merge(struct node *n1, struct node *n2)
{
    struct node *root;

    char *s = calloc(sizeof(char), strlen(n1->data.s) +
        strlen(n2->data.s) + 1);

    strcat(s, n1->data.s);
    strcat(s, n2->data.s);

    root = n_alloc(s, n1->data.f + n2->data.f);
    n_add_left(root, n1);
    n_add_right(root, n2);

    free(s);

    return root;
}

void n_shift(struct node **n, int pos, unsigned int len)
{
    unsigned int i;
    struct node *buff = n[pos];

    for (i = pos; i < len - 1; i++)
        n[i] = n[i + 1];

    n[len - 1] = buff;
}

bool n_is_leaf(struct node *n)
{
    return !n->left && !n->right;
}

void n_set(struct node *n, char *s, int f)
{
    n->data.s = realloc(n->data.s, sizeof(char) * (strlen(s) + 1));
    strcpy(n->data.s, s);
    n->data.f = f;
}
