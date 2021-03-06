#include "hashmap.h"
#include "plugins.h"
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

void ctcp(struct bot *bot, struct message *msg) {
	char *strptr;
	char *arg = strdup(msg->arg);
	char *host = strdup(msg->host);

	char *nick = strtok_r(host, "!", &strptr);
	nick++;

	strptr = NULL;
	char *chan = strtok_r(arg, " ", &strptr);
	char *message = strtok_r(NULL, "", &strptr);
	message++;

	if (message[0] == '\x01') {
		char *ctcp = strtok_r(message+1, "\x01", &strptr);
		if (strcmp(ctcp, "VERSION") == 0) {
			sockprintf(bot->socket, "NOTICE %s \001%s apollo version 1.2 - svkampen\001\r\n", nick, ctcp);
		}
	}

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

void quit_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	bot->running = 0;
}

void plugs_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	char **keys = hashmap_keys(bot->plugins);
	int key_str_len = 0;
	for (int i = 0; i < bot->plugins->length; ++i) {
		key_str_len += strlen(keys[i]);
		key_str_len += 2;
	}
	key_str_len--;

	char *key_str = calloc(key_str_len, 1);
	int key_str_idx = 0;

	for (int i = 0; i < bot->plugins->length; ++i) {
		if (i == (bot->plugins->length - 1)) {
			key_str_idx += sprintf(key_str + key_str_idx, "%s", keys[i]);
		} else {
			key_str_idx += sprintf(key_str+key_str_idx, "%s, ", keys[i]);
		}
	}

	printf("%s\n", key_str);
	bot->proto->msg(chan, "Loaded plugins: %s", key_str);
	free(keys);
	free(key_str);
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

void pload_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: pload <plugin>");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		giggle(bot, chan);
		return;
	}

	load_plugin(bot, args);
	bot->proto->msg(chan, "Loaded plugin %s.", args);
}

void puload_cmd(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: puload <plugin>");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		giggle(bot, chan);
		return;
	}

	void *plug = NULL;

	if ((plug = hashmap_get(args, bot->plugins)) == NULL) {
		bot->proto->msg(chan, "Plugin %s is not currently loaded!");
		return;
	}

	__unload_plugin(args, plug);
	hashmap_remove(args, bot->plugins);
	bot->proto->msg(chan, "Unloaded plugin %s.", args);
}


void init(struct bot *bot) {
    // initialize the plugin

    Link *l = hashmap_get("NOTICE", bot->handlers);
    if (!l) {
        l = calloc(1, sizeof(Link));
        hashmap_set("NOTICE", l, bot->handlers);
    }
    push_val(l, (void*)notice);

	l = hashmap_get("PRIVMSG", bot->handlers);
	if (!l) {
		l = calloc(1, sizeof(Link));
		hashmap_set("PRIVMSG", l, bot->handlers);
	}
	push_val(l, (void*)ctcp);

	hashmap_set("say", say_cmd, bot->commands);
	hashmap_set("join", join_cmd, bot->commands);
	hashmap_set("part", part_cmd, bot->commands);
	hashmap_set("sh", bash_cmd, bot->commands);

	hashmap_set("plugins", plugs_cmd, bot->commands);
	hashmap_set("pload", pload_cmd, bot->commands);
	hashmap_set("puload", puload_cmd, bot->commands);
	hashmap_set("quit", quit_cmd, bot->commands);
}

void destroy(struct bot *bot) {
	hashmap_remove("say", bot->commands);
	hashmap_remove("join", bot->commands);
	hashmap_remove("part", bot->commands);
	hashmap_remove("sh", bot->commands);

	hashmap_remove("plugins", bot->commands);
	hashmap_remove("pload", bot->commands);
	hashmap_remove("puload", bot->commands);
	hashmap_remove("quit", bot->commands);

	Link *l = hashmap_get("NOTICE", bot->handlers);

	if (!l) {
		return;
	}
	free_list(l);
}
