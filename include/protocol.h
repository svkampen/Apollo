#ifndef _APOLLO_PROTOCOL_H_
#define _APOLLO_PROTOCOL_H_

struct bot;

struct apollo_protocol {
	void (*init)(struct bot*);
	void (*connect)();
	void (*tick)();
	void (*destroy)();

	void *protolib;
};

struct protocol_message {
	char *nick;
	char *context;
	char *msg;
	void (*reply)(struct protocol_message *to, char *with);
	void (*pm_reply)(struct protocol_message *to, char *with);
};

void load_protocol(struct bot*);

#include "bot.h"
#endif
