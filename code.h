#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CHAR_LENGTH 8

struct c_data {
    char c;
    int f;
    char *code;
};

struct c_list {
    struct c_data *data;
    struct c_list *next;
};

struct c_block {
    char *bytes;
    struct c_list *c_l;
};

struct c_block * c_bl_alloc(int h);
void c_bl_free(struct c_block *c_bl);

struct c_list * c_l_alloc(void);
void c_l_append(struct c_list *head, struct c_data *data, int n);
void c_l_push(struct c_list *list, struct c_data *data);
void c_l_print(struct c_list *head, int h, bool debug);
void c_l_free(struct c_list *head);

#endif /* CODE_H */
