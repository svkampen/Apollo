#ifndef _BOT_H_
#define _BOT_H_
#include <inttypes.h>
#include "hashmap.h"
#include "cJSON.h"
#include "protocol.h"

#define BUFSIZE 3072

struct bot {
	/** NETWORKING **/
	int socket;
	char buffer[BUFSIZE];
	
	char *admin;
	uint8_t running;

	struct hashmap *handlers;
	struct hashmap *commands;
	struct hashmap *plugins;

	char *host, *port, *nick;

	cJSON *config;
	struct apollo_protocol *proto;
};

#endif
