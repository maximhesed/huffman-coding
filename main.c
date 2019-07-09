#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/* #include <ncurses.h> */

#include "node.h"

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

	struct code *code = calloc(sizeof(struct code), nr_len);

	int k = 0;
	int *arr = calloc(sizeof(int), 256);

	t_get_codes(*n, code, arr, 0, &k);

	/* print codes */
	for (i = 0; i < nr_len; i++) {
		printf("%c: ", code[i].c);

		for (j = 0; j < code[i].len; j++)
			printf("%d", code[i].v[j]);

		printf("\n");
	}

	/*initscr();
	curs_set(0);
	noecho();

	if (!has_colors()) {
		colors_support = false;
		printw("Your terminal does not support colors.\n");
		getch();
	} else {
		colors_support = true;
		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
	}*/

	/* i don't know what else */
	/*int x_shift = strlen(n[0]->data.s) * 2;
	t_print(*n, 0, x_shift, true);

	getch();
	endwin();*/

	t_free(*n);

	/* TODO: c_free(code); */
	for (i = 0; i < nr_len; i++)
		free(code[i].v);

	free(code);

	free(n);
	free(text);
	free(arr);

	return 0;
}
