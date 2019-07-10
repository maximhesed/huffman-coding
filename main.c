#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "node.h"

/* static struct node * n_alloc(char *s, int f); */
static void n_add_left(struct node *root, struct node *n);
static void n_add_right(struct node *root, struct node *n);
static void n_free(struct node *n);
static void n_print(struct node *n);
static void n_sort(struct node **n, unsigned int len);
static struct node * n_merge(struct node *n1, struct node *n2);
static void n_cut(struct node **n, int pos, unsigned int len);
static void t_get_codes(struct node *n, struct code *code, int *arr,
						int offset, int *k);
static bool n_is_leaf(struct node *n);
static void n_set(struct node *n, char *s, int f);
static void t_print(struct node *n, int y, int x, bool codes);

bool colors_support;

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

	struct node **n = calloc(sizeof(struct node), 1);

	unsigned int i;
	int j;
	int nr_len = 0; /* not repeat length */
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
			nr_len++;
			n = realloc(n, sizeof(struct node) * nr_len);

			char text_buff[2];
			text_buff[0] = text[i];
			text_buff[1] = '\0';

			/* using only once */
			/* n[nr_len - 1] = n_alloc(text_buff, 1); */

			n[nr_len - 1] = calloc(sizeof(struct node), 1);
			n_set(n[nr_len - 1], text_buff, 1);
		}
	}

	n_sort(n, nr_len);

	/* create tree */
	i = 0;

	while (i < nr_len - 1) {
		const int offset = 0;

		n[offset] = n_merge(n[offset], n[offset + 1]);
		n_cut(n, 1, nr_len - i);
		i++;

		n_sort(n, nr_len - i);

		/* print nodes */
		for (j = 0; j < nr_len - i; j++)
			n_print(n[j]);

		printf("\n");
	}

	struct code *code = calloc(sizeof(struct code), nr_len);

	for (i = 0; i < nr_len; i++)
		code[i].v = calloc(sizeof(int), 1);

	int k = 0;
	int *arr = calloc(sizeof(int), 256);
	t_get_codes(*n, code, arr, 0, &k);

	for (i = 0; i < nr_len; i++) {
		printf("%c: ", code[i].c);

		for (j = 0; j < code[i].len; j++)
			printf("%d", code[i].v[j]);

		printf("\n");
	}

	initscr();
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
	}

	/* i don't know what else */
	int x_shift = strlen(n[0]->data.s) * 2;

	t_print(*n, 0, x_shift, true);

	getch();
	endwin();

	free(text);
	free(code);
	n_free(*n);

	return 0;
}

/*static struct node * n_alloc(char *s, int f)
{
	struct node *n = calloc(sizeof(struct node), 1);

	n->data.s = calloc(sizeof(char), strlen(s) + 1);
	strcpy(n->data.s, s);
	n->data.f = f;
	n->left = NULL;
	n->right = NULL;

	return n;
}*/

static void n_add_left(struct node *root, struct node *n)
{
	root->left = calloc(sizeof(struct node), 1);
	memcpy(root->left, n, sizeof(struct node));
}

static void n_add_right(struct node *root, struct node *n)
{
	root->right = calloc(sizeof(struct node), 1);
	memcpy(root->right, n, sizeof(struct node));
}

static void n_free(struct node *n)
{
	free(n->data.s);
	free(n);
}

static void n_print(struct node *n)
{
	printf("%s - %d\n", n->data.s, n->data.f);
}

static void n_sort(struct node **n, unsigned int len)
{
	bool sorted = false;

	while (!sorted) {
		unsigned int i;

		sorted = true;

		for (i = 0; i < len - 1; i++) {
			if (n[i]->data.f > n[i + 1]->data.f) {
				struct node *buff;

				sorted = false;

				buff = n[i];
				n[i] = n[i + 1];
				n[i + 1] = buff;
			}
		}
	}
}

static struct node * n_merge(struct node *n1, struct node *n2)
{
	struct node *root = calloc(sizeof(struct node), 1);

	n_add_left(root, n1);
	n_add_right(root, n2);

	unsigned int n1_len = strlen(n1->data.s);
	unsigned int n2_len = strlen(n2->data.s);
	char *s = calloc(sizeof(char), n1_len + n2_len + 1);

	strcat(s, n1->data.s);
	strcat(s, n2->data.s);

	n_set(root, s, n1->data.f + n2->data.f);

	return root;
}

static void n_cut(struct node **n, int pos, unsigned int len)
{
	unsigned int i;

	for (i = pos; i < len - 1; i++) {
		struct node buff;
		memcpy(&buff, n[i], sizeof(struct node));
		memcpy(n[i], n[i + 1], sizeof(struct node));
		memcpy(n[i + 1], &buff, sizeof(struct node));
	}

	for (i = pos; i < len - 1; i++) {
		struct node *buff;

		buff = n[i];
		n[i] = n[i + 1];
		n[i + 1] = buff;
	}

	/* 15 errors here */
	/* n = realloc(n, sizeof(struct node) * (len - 1)); */

	n_free(n[len - 1]);
}

static void t_get_codes(struct node *n, struct code *code, int *arr,
						int offset, int *k)
{
	if (n->left != NULL) {
		arr[offset] = 0;
		t_get_codes(n->left, code, arr, offset + 1, k);
	}

	if (n->right != NULL) {
		arr[offset] = 1;
		t_get_codes(n->right, code, arr, offset + 1, k);
	}

	if (n_is_leaf(n)) {
		int i;

		code[*k].c = n->data.s[0];
		code[*k].v = calloc(sizeof(int), offset);
		code[*k].len = offset;

		for (i = 0; i < offset; i++)
			code[*k].v[i] = arr[i];

		(*k)++;
	}
}

static bool n_is_leaf(struct node *n)
{
	if (strlen(n->data.s) == 1)
		return true;

	return false;
}

static void n_set(struct node *n, char *s, int f)
{
	n->data.s = realloc(n->data.s, sizeof(char) * (strlen(s) + 1));
	strcpy(n->data.s, s);
	n->data.f = f;
}

static void t_print(struct node *n, int y, int x, bool codes)
{
	int x_left = x;
	int x_right = x;

	move(y, x);
	printw("%s", n->data.s);

	if (n->left != NULL) {
		move(++y, --x_left);
		addch('/');

		if (codes) {
			if (colors_support) {
				attron(COLOR_PAIR(1));
				mvaddch(y, x_left - 1, '0');
				attroff(COLOR_PAIR(1));
			} else
				mvaddch(y, x_left - 1, '0');
		}
	}

	if (n->right != NULL) {
		move(y, x_right += strlen(n->data.s));
		addch('\\');

		if (codes) {
			if (colors_support) {
				attron(COLOR_PAIR(1));
				mvaddch(y, x_right + 1, '1');
				attroff(COLOR_PAIR(1));
			} else
				mvaddch(y, x_right + 1, '1');
		}
	}

	if (n->left != NULL)
		t_print(n->left, ++y, x_left -= strlen(n->left->data.s), codes);

	if (n->right != NULL)
		t_print(n->right, y, ++x_right, codes);
}
