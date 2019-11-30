#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "node.h"
#include "code.h"
#include "tree.h"

int main(int argc, char *argv[])
{
    struct node **n = NULL;
    unsigned int i;
    unsigned nr_len = 0; /* not repeat length */
    bool repeat;
    unsigned int tp_q; /* tree's pieces quantity */
    int h;
    struct c_block *c_bl = NULL;
    char *f_str = NULL; /* file, contains source text */
    FILE *f = NULL;
    int c;

    if (argc != 2) {
        fprintf(stderr, "usage: %s fname\n", argv[0]);

        return -1;
    }

    f_str = strdup(argv[1]);

    f = fopen(f_str, "r");
    if (!f) {
        fprintf(stderr, "couldn't open file \"%s\"\n", f_str);

        free(f_str);

        return -1;
    }

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n')
            continue;

        repeat = false;

        for (i = 0; i < nr_len; i++) {
            if (n[i]->data.s[0] == c) {
                repeat = true;

                n[i]->data.f++;
            }
        }

        if (!repeat) {
            char text_buff[2] = {
                c, '\0'
            };

            nr_len++;
            n = realloc(n, sizeof(struct node *) * nr_len);
            n[nr_len - 1] = n_alloc(text_buff, 1);
        }
    }

    n_sort(n, nr_len);

    /* create tree */
    tp_q = nr_len;

    while (tp_q > 1) {
        n[0] = n_merge(n[0], n[0 + 1]);
        n_shift(n, 1, tp_q);
        n_sort(n, tp_q - 1);

        tp_q--;
    }

    /* print sorted frequencies */
    /*for (i = 0; i < nr_len; i++)
        printf("'%c'.freq = %d\n", n[i]->data.s[0], n[i]->data.f);*/

    h = t_get_height(*n) - 1;
    c_bl = c_bl_alloc(h);

    t_get_codes(*n, c_bl, 0, h);

    /* print codes */
    c_l_print(c_bl->c_l, h, false);

    /* free */
    free(f_str);
    fclose(f);

    t_free(*n);
    free(n);

    c_bl_free(c_bl);

    return 0;
}
