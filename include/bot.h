#ifndef _BOT_H_
#define _BOT_H_
#include <inttypes.h>
#include "hashmap.h"
#include "cJSON.h"
#include "protocol.h"
#include "vector.h"

#define BUFSIZE 3072

struct string_list {
	int size;
	int index;
	char **list;
};

struct bot {
	/** NETWORKING **/
	int socket;
	char buffer[BUFSIZE];

	char *admin;
	uint8_t running;

	struct hashmap *handlers;
	struct hashmap *commands;
	struct hashmap *plugins;
	struct string_list *channels;

	struct double_link *tick_functions;

	char *host, *port, *nick;

	cJSON *config;
	struct apollo_protocol *proto;
};

#endif
