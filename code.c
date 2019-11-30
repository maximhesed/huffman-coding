#include "code.h"

static void c_l_sort(struct c_list *head);

static void c_l_sort(struct c_list *head)
{
    bool sorted;
    struct c_list *curr;
    struct c_list *lptr = NULL;

    if (head == NULL)
        return;

    do {
        sorted = true;
        curr = head;

        while (curr->next != lptr) {
            if (curr->data->f < curr->next->data->f) {
                struct c_data *buf;

                buf = curr->data;
                curr->data = curr->next->data;
                curr->next->data = buf;

                sorted = false;
            }

            curr = curr->next;
        }

        lptr = curr;
    } while (!sorted);
}

struct c_block * c_bl_alloc(int h)
{
    struct c_block *c_bl = malloc(sizeof(struct c_block));

    /* (size + 1) is tree height - max code length in the tree.
     * It is needed, that don't do realloc every time in
     * t_get_codes().
     *
     * Only one c_bl->bytes is used, during all tree traversal. */
    c_bl->bytes = calloc(sizeof(char), h + 1);

    c_bl->c_l = c_l_alloc();

    return c_bl;
}

void c_bl_free(struct c_block *c_bl)
{
    free(c_bl->bytes);
    c_l_free(c_bl->c_l);

    free(c_bl);
}

struct c_list * c_l_alloc(void)
{
    struct c_list *c_l = malloc(sizeof(struct c_list));

    c_l->data = malloc(sizeof(struct c_data));
    c_l->data->c = 0;
    c_l->data->f = 0;
    c_l->data->code = NULL;

    c_l->next = NULL;

    return c_l;
}

/* somewhat awful... */
void c_l_append(struct c_list *head, struct c_data *data, int n)
{
    struct c_list *tmp;

    if (head->data->c == 0)
        c_l_push(head, data);
    else {
        tmp = head;

        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = c_l_alloc();
        c_l_push(tmp->next, data);
    }
}

void c_l_push(struct c_list *list, struct c_data *data)
{
    list->data->c = data->c;
    list->data->f = data->f;
    list->data->code = strdup(data->code);
}

void c_l_print(struct c_list *head, int h, bool debug)
{
    /* sort codes by frequency */
    c_l_sort(head);

    if (debug)
        printf("%c: %s (f: %d; w: %d)\n",
            head->data->c, head->data->code, head->data->f,
            head->data->f * CHAR_LENGTH - head->data->f *
                strlen(head->data->code));
    else
        printf("%c: %s\n", head->data->c, head->data->code);

    if (head->next != NULL)
        c_l_print(head->next, h, debug);
}

void c_l_free(struct c_list *head)
{
    struct c_list *tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;

        free(tmp->data->code);
        free(tmp->data);
        free(tmp);
    }
}
