#ifndef _APOLLO_PROTOCOL_H_
#define _APOLLO_PROTOCOL_H_
#include <stdarg.h>

struct bot;

struct apollo_protocol {
	char *name;
	void (*init)(struct bot*);
	void (*connect)();
	void (*tick)();
	void (*destroy)();

	void (*msg)(char *ctx, const char *fmt, ...);
	void (*pm_msg)(char *user, const char *fmt, ...);

	void *protolib;
};

void load_protocol(struct bot*);

#include "bot.h"
#endif
