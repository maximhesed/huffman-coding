#include "node.h"

static int max(int a, int b);

static int max(int a, int b)
{
	return ((a > b) ? a : b);
}

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

/* TODO: try quick sort */
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
	return !n->left && !n->right;
}

void n_set(struct node *n, char *s, int f)
{
	n->data.s = realloc(n->data.s, sizeof(char) * (strlen(s) + 1));
	strcpy(n->data.s, s);
	n->data.f = f;
}

void t_get_codes(struct node *root, struct c_block *c_bl, int index, int h)
{
	if (root->left != NULL) {
		c_bl->b[index] = '0';
		t_get_codes(root->left, c_bl, index + 1, h);
	}

	if (root->right != NULL) {
		c_bl->b[index] = '1';
		t_get_codes(root->right, c_bl, index + 1, h);
	}

	if (n_is_leaf(root))
		c_l_append(c_bl->c_l, root->data.s[0], c_bl->b, h);
}

int t_get_height(struct node *root)
{
	if (root == NULL)
		return 0;

	int h_l = t_get_height(root->left);
	int h_r = t_get_height(root->right);

	return max(h_l, h_r) + 1;
}

void t_free(struct node *root)
{
	if (root == NULL)
		return;

	t_free(root->left);
	t_free(root->right);

	n_free(root);
}
