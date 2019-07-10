#ifndef NODE_H
#define NODE_H

struct data {
	char *s;
	int f;
};

struct node {
	struct data data;
	struct node *left;
	struct node *right;
};

struct code {
	char c;
	int *v;
	int len;
};

#endif /* NODE_H */
