#include "handlers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void privmsg(struct bot *bot, struct message *msg) {
	char *strptr;
	char *arg = strdup(msg->arg);

	char *nick = strtok_r(msg->host, "!", &strptr);
	nick++; // skip the :

	strptr = NULL;

	char *chan = strtok_r(arg, " ", &strptr);
	char *message = strtok_r(NULL, ":", &strptr);

	printf("[%s] <%s> %s\n", chan, nick, message);

	if (EQ(nick, "svkampen") && EQ(message, "'quit")) {
		bot->running = 0;
	}

	free(arg);
}

