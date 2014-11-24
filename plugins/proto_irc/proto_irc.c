#include "protocol.h"
#include "bot.h"
#include "net.h"
#include "hashmap.h"
#include "proto_irc_parse.h"
#include "proto_irc_handlers.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static struct bot *bot;
#define EQ(a, b) (strcmp(a, b) == 0)
typedef void (*handler_t)(struct bot*, struct message*);


void proto_reply(char *ctx, const char *fmt, ...) {
	va_list a;
	va_start(a, fmt);

	char *sockprintf_format = calloc(1, strlen(fmt) + strlen(ctx) + 11);

	sprintf(sockprintf_format, "PRIVMSG %s :%s", ctx, fmt);
	vssockprintf(bot->socket, sockprintf_format, a);
	free(sockprintf_format);
	va_end(a);
}

void proto_pm_reply(char *user, const char *fmt, ...) {
	va_list a;
	va_start(a, fmt);

	char *sockprintf_format = calloc(1, strlen(fmt) + strlen(user) + 11);

	sprintf(sockprintf_format, "PRIVMSG %s :%s", user, fmt);
	vssockprintf(bot->socket, sockprintf_format, a);
	free(sockprintf_format);
	va_end(a);
}


void proto_init(struct bot *b) {
	bot = b;
	Link *invite_link = hashmap_get("INVITE", b->handlers);
	if (!invite_link) {
		invite_link = calloc(1, sizeof(Link));
		hashmap_set("INVITE", invite_link, b->handlers);
	}

	push_val(invite_link, (void*)irc_invite);

	Link *privmsg_link = hashmap_get("PRIVMSG", b->handlers);
	if (!privmsg_link) {
		privmsg_link = calloc(1, sizeof(Link));
		hashmap_set("PRIVMSG", privmsg_link, b->handlers);
	}

	push_val(privmsg_link, (void*)irc_privmsg);
}

void proto_connect() {
	printf("[irc\tinfo] registering with the server.\n");
	sockprintf(bot->socket, "NICK %s", bot->nick);
	sockprintf(bot->socket, "USER apollo * * :hi i'm apollo");
}

void proto_tick() {
	char *i = irc_getline(bot);
	if (i == NULL) {
		return;
	}

	for (; i != NULL; i = irc_nextline(bot)) {
		
		printf("%s\n", i);

		if (startswith(i, "PING")) {
			char *t = last(i);
			sockprintf(bot->socket, "PONG %s", t);
			free(t);
			free(i);
			continue;
		}

		struct message *msg = parse(i);

		if (EQ(msg->meth, "422") || EQ(msg->meth, "376")) {
			sockprintf(bot->socket, "JOIN #bots");
		}

		Link *handlers = hashmap_get(msg->meth, bot->handlers);
		for (Link *i = handlers; i != NULL; i = i->next) {
			((handler_t)i->ptr)(bot, msg);
		}

		freemsg(msg);
		free(i);
	}
}

void proto_destroy() {
	Link *invite_link = hashmap_get("INVITE", bot->handlers);
	if (invite_link) {
		free_list(invite_link);
	}

	Link *privmsg_link = hashmap_get("PRIVMSG", bot->handlers);
	if (privmsg_link) {
		free_list(privmsg_link);
	}
}

struct apollo_protocol proto = {proto_init, proto_connect, proto_tick, proto_destroy, proto_reply, proto_pm_reply};
