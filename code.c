#include "code.h"

struct c_block * c_bl_alloc(int size)
{
    struct c_block *c_bl = malloc(sizeof(struct c_block));

    c_bl->bytes = calloc(sizeof(char), size + 1);
    c_bl->c_l = c_l_alloc(size);

    return c_bl;
}

void c_bl_free(struct c_block *c_bl)
{
    free(c_bl->bytes);
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

/* somewhat awful... */
void c_l_append(struct c_list *head, char c, char *code, int n)
{
    if (head->c == 0)
        c_l_push(head, c, code, n);
    else {
        struct c_list *tmp = head;

        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = c_l_alloc(n);
        c_l_push(tmp->next, c, code, n);
    }
}

void c_l_push(struct c_list *list, char c, char *code, int n)
{
    list->c = c;
    strncpy(list->code, code, n);
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
