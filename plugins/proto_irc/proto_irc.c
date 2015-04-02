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
#include <errno.h>
#include <netinet/in.h> // INET6_ADDRSTRLEN
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>


static struct bot *bot;
#define EQ(a, b) (strcmp(a, b) == 0)
typedef void (*handler_t)(struct bot*, struct message*);
typedef void (*tick_fn)(struct bot*);


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
	{
		struct addrinfo *info = get_addr_info(bot->host, bot->port);

		bot->socket = getsock(info);
		fcntl(bot->socket, F_SETFL, O_NONBLOCK);
		bot->running = 1;

		// we won't need this anymore.
		freeaddrinfo(info);
	}

	printf("[irc\tinfo] registering with the server.\n");
	sockprintf(bot->socket, "NICK %s", bot->nick);
	sockprintf(bot->socket, "USER apollo * * :hi i'm apollo");
}

void tick_functions() {
	// run all of the tick-based functions (fifo_tick, et cetera)
	Link *tick_based = bot->tick_functions;
	for (Link *i = bot->tick_functions; i != NULL; i = i->next) {
		if (i->ptr)
			((tick_fn)i->ptr)(bot);
	}
}

void proto_tick() {
	int nbytes;
	char data[BUFSIZE/2];

	while (bot->running) {
		memset(&data, 0, BUFSIZE/2);

tick:
		tick_functions();

		recv:
		if ((nbytes = recv(bot->socket, data, (BUFSIZE/2)-1, 0)) == -1) {
			if (errno == EINTR) {
				// what a pain in the ass,
				// we got interrupted >.>
				goto recv;
			}

			if (errno == EAGAIN) {
				usleep(16666);
				goto tick;
			}


			perror("recv failed");
			exit(4);
		}

		strcat(bot->buffer, data);

		if (nbytes == 0) {
			bot->running = 0;
		}

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
}

void proto_destroy() {
	close(bot->socket);

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
