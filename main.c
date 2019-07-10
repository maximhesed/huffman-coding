#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "node.h"
#include "code.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("type --help for more information\n");
		return -1;
	}

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		printf("usage: ./prog <text>\n");
		return -1;
	}

	unsigned int len = strlen(argv[1]);
	char *text = calloc(sizeof(char), len + 1);
	strcpy(text, argv[1]);

	struct node **n = NULL;

	unsigned int i;
	unsigned int j;
	unsigned int nr_len = 0; /* not repeat length */
	bool repeat;

	for (i = 0; i < len; i++) {
		repeat = false;

		for (j = 0; j < nr_len; j++) {
			if (n[j]->data.s[0] == text[i]) {
				repeat = true;
				n[j]->data.f++;
			}
		}

		if (!repeat) {
			char text_buff[2] = {
				text[i], '\0'
			};

			nr_len++;
			n = realloc(n, sizeof(struct node *) * nr_len);
			n[nr_len - 1] = n_alloc(text_buff, 1);
		}
	}

	n_sort(n, nr_len);

	/* create tree */
	unsigned int tp_q = nr_len; /* tree's pieces quantity */

	while (tp_q > 1) {
		const int offset = 0;

		n[offset] = n_merge(n[offset], n[offset + 1]);
		n_shift(n, 1, tp_q);
		n_sort(n, tp_q - 1);

		tp_q--;
	}

	int h = t_get_height(*n) - 1;
	struct c_block *c_bl = c_bl_alloc(h);

	t_get_codes(*n, c_bl, 0, h);

	/* print codes */
	c_l_print(c_bl->c_l, h);

	free(text);

	t_free(*n);
	free(n);

	c_bl_free(c_bl);

	return 0;
}
