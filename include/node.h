#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

#include "code.h"

struct data {
    char *s;
    int f;
};

struct node {
    struct data data;
    struct node *left;
    struct node *right;
};

struct node * n_alloc(char *s, int f);
void n_add_left(struct node *root, struct node *n);
void n_add_right(struct node *root, struct node *n);
void n_free(struct node *n);
void n_print(struct node *n);
void n_sort(struct node **n, unsigned int len);
struct node * n_merge(struct node *n1, struct node *n2);
void n_shift(struct node **n, int pos, unsigned int len);
bool n_is_leaf(struct node *n);
void n_set(struct node *n, char *s, int f);

#endif /* NODE_H */
