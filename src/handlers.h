#ifndef _HANDLERS_H_
#define _HANDLERS_H_
#include "bot.h"
#include "parse.h"
#include "link.h"

#define EQ(a, b) (strcmp(a, b) == 0)

struct handler_list {
	char *handler_type;
	link *list;
}

link *get_handlers(char *type);
void add_handler(char *type, void *func);

#endif
