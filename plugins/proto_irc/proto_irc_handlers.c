#include "proto_irc_handlers.h"
#include "proto_irc_parse.h"

#include <stdlib.h>
#include <string.h>
#include "net.h"

#define EQ(a, b) (strcmp(a, b) == 0)
typedef void (*msg_handler)(struct bot*, char*, char*, char*);

void irc_privmsg(struct bot *bot, struct message *msg) {
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
		bot->running = 0;
	}

	if (startswith(message, "'")) {
		char *cmd = strtok_r(message+1, " ", &strptr);
		char *args = strtok_r(NULL, "", &strptr);

		msg_handler callback = (msg_handler)hashmap_get(cmd, bot->commands);
		if (callback) {
			callback(bot, nick, chan, args);
		}
	}

	free(arg);
	free(host);
}

void irc_invite(struct bot *bot, struct message *msg) {
	char *args = strdup(msg->arg);
	char *chan = last(args);

	sockprintf(bot->socket, "JOIN :%s", chan+1);

	free(args);
	free(chan);
}
