#ifndef _BOT_H_
#define _BOT_H_
#include <inttypes.h>

#define BUFSIZE 1024
struct bot {
	int socket;
	uint8_t running;
	char buffer[BUFSIZE];
};
#endif
