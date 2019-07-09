#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>

struct c_list {
	char c;
	int *code;
	struct c_list *next;
};

struct c_block {
	int *b;
	struct c_list *c_l;
};

struct c_block * c_bl_alloc(int h);
void             c_bl_free(struct c_block *c_bl);

struct c_list * c_l_alloc(int h);
void            c_l_append(struct c_list *head, char c, int *code, int h);
void            c_l_push(struct c_list *list, char c, int *code, int h);
void            c_l_print(struct c_list *head, int h);
void            c_l_free(struct c_list *head);

#endif /* CODE_H */
