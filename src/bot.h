#ifndef _BOT_H_
#define _BOT_H_
#include <inttypes.h>
#include "hashmap.h"

#define BUFSIZE 3072

struct bot {
	int socket;
	struct hashmap *handlers;
	uint8_t running;
	char buffer[BUFSIZE];
};
#endif
