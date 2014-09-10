#ifndef _BOT_H_
#define _BOT_H_
#include <inttypes.h>
#include "hashmap.h"

#define BUFSIZE 3072

struct bot {
	int socket;
	char *admin;
	struct hashmap *handlers;
	struct hashmap *commands;
	struct hashmap *plugins;
	struct hashmap *data;
	int *x;
	uint8_t running;
	char buffer[BUFSIZE];

	void (*msg)(struct bot *, char *, char *);
};

#endif
