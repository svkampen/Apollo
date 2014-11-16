#include "hashmap.h"
#include "bot.h"
#include "message.h"
#include "net.h"
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
    printf("[common\tinfo] -%s- %s\n", nick, message);
    
    free(arg);
    free(host);
}

void giggle(struct bot *bot, char *chan) {
	bot->proto->msg(chan, "\x01" "ACTION giggles" "\x01");
}


void say_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: say <args> -> tells you sth");
		return;
	}

	bot->proto->msg(chan, args);
}

void join_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: join <channel>");
		return;
	}

	if (args[0] == '0') {
		giggle(bot, chan);
		return;
	}

	sockprintf(bot->socket, "JOIN :%s", args);
}

void part_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	sockprintf(bot->socket, "PART :%s", chan);
}

void bash_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: sh <command>");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		giggle(bot, chan);
		return;
	}

	char *command = calloc(1, strlen(args) + 11);

	strcat(command, "bash -c \"");
	strcat(command, args);
	strcat(command, "\"");

	char line[1024];

	FILE *process = popen((const char*)command, "r");
	while (fgets(line, 1024, process) != NULL) {
		bot->proto->msg(chan, line);
	}

	pclose(process);
	free(command);
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
	hashmap_set("join", join_cmd, bot->commands);
	hashmap_set("part", part_cmd, bot->commands);
	hashmap_set("sh", bash_cmd, bot->commands);
}

void destroy(struct bot *bot) {
	Link *l = hashmap_get("NOTICE", bot->handlers);

	if (!l) {
		return;
	}
	free_list(l);
}
