#include "hashmap.h"
#include "bot.h"
#include "message.h"
#include <stdlib.h>
#include <string.h>

void notice(struct bot *bot, struct message *msg) {
    char *strptr;
    char *arg = strdup(msg->arg);
    char *host = strdup(msg->host);
    
    char *nick = strtok_r(host, "!", &strptr);
    nick++;
    strptr = NULL;
	char *chan = strtok_r(arg, " ", &strptr);
    char *message = strtok_r(NULL, ":", &strptr);
    printf("[notice] -%s- %s\n", nick, message);
    
    free(arg);
    free(host);
}

void say_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->msg(bot, chan, "usage: say <args> -> tells you sth");
		return;
	}

	char *output = calloc(1, 4+strlen(nick)+strlen(args));
	sprintf(output, "<%s> %s", nick, args);
	bot->msg(bot, chan, output);
	free(output);
}

void init(struct bot *bot) {
    // initialize the plugin
    Link *l = hashmap_get("NOTICE", bot->handlers);
    if (!l) {
        l = calloc(1, sizeof(Link));
        hashmap_set("NOTICE", l, bot->handlers);
    }
    push_val(l, (void*)notice);

	hashmap_set("say", say_cmd, bot->commands);
}
