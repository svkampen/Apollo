#include "handlers.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*callback)(struct bot*, char*, char*, char*);

void privmsg(struct bot *bot, struct message *msg) {
	char *strptr;
	char *arg = strdup(msg->arg);
	char *host = strdup(msg->host);

	char *nick = strtok_r(host, "!", &strptr);
	nick++; // skip the :

	strptr = NULL;

	char *chan = strtok_r(arg, " ", &strptr);
	char *message = strtok_r(NULL, "", &strptr);
	message++;

	printf("[%s] <%s> %s\n", chan, nick, message);

	if (EQ(nick, "svkampen") && EQ(message, "'quit")) {
		bot->running = 0;
	}

	char *cmd = strtok_r(message+1, " ", &strptr);
	char *args = strtok_r(NULL, "", &strptr);

	callback cb = (callback)hashmap_get(cmd, bot->commands);
	if (cb) {
		cb(bot, nick, chan, args);
	}
	
	
	free(arg);
	free(host);
}

