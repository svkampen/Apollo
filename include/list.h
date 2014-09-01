#ifndef _LIST_H_
#define _LIST_H_
#include <stdlib.h>
#include <stdio.h>

/* MACROS */
#define LINK_CREATE(X) struct double_link *X = calloc(1, sizeof(struct double_link))

struct double_link {
	struct double_link *prev;
	struct double_link *next;
	void *ptr;
};

struct double_link *push_link_top(struct double_link *list, struct double_link *link);
struct double_link *push_link(struct double_link *list, struct double_link *link);
void push_val(struct double_link *list, void *value);
void remove_link(struct double_link *link);
void free_list(struct double_link *list);
struct double_link *get_list_head(struct double_link *list);

#endif
