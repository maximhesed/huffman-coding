#include "node.h"

struct node * n_alloc(char *s, int f)
{
	struct node *n = malloc(sizeof(struct node));

	n->data.s = calloc(sizeof(char), strlen(s) + 1);
	strcpy(n->data.s, s);
	n->data.f = f;
	n->left = NULL;
	n->right = NULL;

	return n;
}

void n_add_left(struct node *root, struct node *n)
{
	root->left = n;
}

void n_add_right(struct node *root, struct node *n)
{
	root->right = n;
}

void n_free(struct node *n)
{
	free(n->data.s);
	free(n);
}

void n_print(struct node *n)
{
	printf("%s - %d\n", n->data.s, n->data.f);
}

void n_sort(struct node **n, unsigned int len)
{
	bool sorted = false;

	while (!sorted) {
		unsigned int i;

		sorted = true;

		for (i = 0; i < len - 1; i++) {
			if (n[i]->data.f > n[i + 1]->data.f) {
				struct node *buff;

				buff = n[i];
				n[i] = n[i + 1];
				n[i + 1] = buff;

				sorted = false;
			}
		}
	}
}

struct node * n_merge(struct node *n1, struct node *n2)
{
	struct node *root;

	char *s = calloc(sizeof(char), strlen(n1->data.s) +
		strlen(n2->data.s) + 1);

	strcat(s, n1->data.s);
	strcat(s, n2->data.s);

	root = n_alloc(s, n1->data.f + n2->data.f);
	n_add_left(root, n1);
	n_add_right(root, n2);

	free(s);

	return root;
}

void n_shift(struct node **n, int pos, unsigned int len)
{
	unsigned int i;
	struct node *buff = n[pos];

	for (i = pos; i < len - 1; i++)
		n[i] = n[i + 1];

	n[len - 1] = buff;
}

bool n_is_leaf(struct node *n)
{
	return !n->left | !n->right; /* there are no rays */
}

void n_set(struct node *n, char *s, int f)
{
	n->data.s = realloc(n->data.s, sizeof(char) * (strlen(s) + 1));
	strcpy(n->data.s, s);
	n->data.f = f;
}

/* TODO: crop excess bytes */
void t_get_codes(struct node *n, struct c_block *c_bl, int index, int h)
{
	if (n->left != NULL) {
		c_bl->b[index] = 0;
		t_get_codes(n->left, c_bl, index + 1, h);
	}

	if (n->right != NULL) {
		c_bl->b[index] = 1;
		t_get_codes(n->right, c_bl, index + 1, h);
	}

	if (n_is_leaf(n))
		c_l_append(c_bl->c_l, n->data.s[0], c_bl->b, h);
}

int t_get_height(struct node *root)
{
	if (root == NULL)
		return 0;

	int h_l = t_get_height(root->left);
	int h_r = t_get_height(root->right);

	if (h_l > h_r)
		return h_l + 1;
	else
		return h_r + 1;
}

void t_free(struct node *n)
{
	if (n == NULL)
		return;

	t_free(n->left);
	t_free(n->right);

	n_free(n);
}

/*void t_print(struct node *n, int y, int x, bool codes)
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
}*/
