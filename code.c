#include "code.h"

struct c_block * c_bl_alloc(int h)
{
	struct c_block *c_bl = malloc(sizeof(struct c_block));

	c_bl->b = calloc(sizeof(char), h + 1);
	c_bl->c_l = c_l_alloc(h);

	return c_bl;
}

void c_bl_free(struct c_block *c_bl)
{
	free(c_bl->b);
	c_l_free(c_bl->c_l);

	free(c_bl);
}

struct c_list * c_l_alloc(int h)
{
	struct c_list *c_l = malloc(sizeof(struct c_list));

	c_l->c = 0;
	c_l->code = calloc(sizeof(char), h + 1);
	c_l->next = NULL;

	return c_l;
}

void c_l_append(struct c_list *head, char c, char *code, int h)
{
	if (head->c == 0)
		c_l_push(head, c, code, h);
	else {
		struct c_list *tmp = head;

		while (tmp->next != NULL)
			tmp = tmp->next;

		tmp->next = c_l_alloc(h);
		c_l_push(tmp->next, c, code, h);
	}
}

void c_l_push(struct c_list *list, char c, char *code, int h)
{
	list->c = c;
	strcpy(list->code, code);
}

void c_l_print(struct c_list *head, int h)
{
	printf("%c: %s\n", head->c, head->code);

	if (head->next != NULL)
		c_l_print(head->next, h);
}

void c_l_free(struct c_list *head)
{
	struct c_list *tmp;

	while (head != NULL) {
		tmp = head;
		head = head->next;

		free(tmp->code);
		free(tmp);
	}
}
