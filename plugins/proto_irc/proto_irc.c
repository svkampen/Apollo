#include "protocol.h"
#include "bot.h"
#include "net.h"
#include "hashmap.h"
#include "proto_irc_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static struct bot *bot;
#define EQ(a, b) (strcmp(a, b) == 0)

void proto_reply(char *context, const char *fmt, ...) {
	va_list a;
	va_start(a, fmt);

	char *sockprintf_format = calloc(1, strlen(fmt) + strlen(context) + 11);

	sprintf(sockprintf_format, "PRIVMSG %s :%s", context, fmt);
	vssockprintf(bot->socket, sockprintf_format, a);
	free(sockprintf_format);
	va_end(a);
}

void proto_pm_reply(char *context, const char *fmt, ...) {
	va_list a;
	va_start(a, fmt);

	char *sockprintf_format = calloc(1, strlen(fmt) + strlen(context) + 11);

	sprintf(sockprintf_format, "PRIVMSG %s :%s", context, fmt);
	vssockprintf(bot->socket, sockprintf_format, a);
	free(sockprintf_format);
	va_end(a);
}

void proto_privmsg(struct message *msg) {
	char *strptr;
	char *arg = strdup(msg->arg);
	char *host = strdup(msg->host);

	char *nick = strtok_r(host, "!", &strptr);
	nick++;

	strptr = NULL;

	char *chan = strtok_r(arg, " ", &strptr);
	char *message = strtok_r(NULL, "", &strptr);
	message++;

	printf("[irc\tinfo] [%s] <%s> %s\n", chan, nick, message);

	if (EQ(nick, "svkampen") && EQ(message, "'quit")) {
		proto_reply(chan, "..closing socket %d", bot->socket);		
		bot->running = 0;
	}

	/* if (startswith(message, "'")) {
		char *cmd = strtok_r(message+1, " ", &strptr);
		char *args = strtok_r(NULL, "", &strptr);

		callback cb = (callback)hashmap_get(cmd, bot->commands);
		if (cb) {
			cb(bot, nick, chan, args);
		}
	} */

	free(arg);
	free(host);
}

void proto_init(struct bot *b) {
	bot = b;
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

		if (EQ(msg->meth, "PRIVMSG")) {
			proto_privmsg(msg);
		}
		
		/* ll_link *handlers = hashmap_get(msg->meth, b.handlers);
		for (ll_link *i = handlers; i != NULL; i = i->next) {
			((handler_t)i->ptr)(&b, msg);
		} */


		freemsg(msg);
		free(i);
	}
}
	

void proto_destroy() { ; }
	

struct apollo_protocol proto = {proto_init, proto_connect, proto_tick, proto_destroy};