#ifndef TREE_H
#define TREE_H

#include <ncurses.h>

#include "node.h"
#include "code.h"

void t_get_codes(struct node *root, struct c_block *c_bl, int index, int h);
int t_get_height(struct node *root);
void t_free(struct node *root);
void t_print(struct node *n, int y, int x, bool codes);

#endif /* TREE_H */
