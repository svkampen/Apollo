#include "hashmap.h"
#include "plugins.h"
#include "bot.h"
#include "message.h"
#include "net.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *bf_lib_handle;
char *(*interpret)(const char*);

void cmd_bf(struct bot *bot, char *nick, char *chan, char *arg) {
	char *output = interpret(arg);
	bot->proto->msg(chan, "%s", output);
	free(output);
}

void init(struct bot *bot) {
	bf_lib_handle = dlopen("./plugins/libbci.so", RTLD_NOW);
	if (bf_lib_handle == NULL) {
		printf("[bf\tfail] Error loading shared library: libbci.so\n");
		exit(0);
	}

	*(void **) (&interpret) = dlsym(bf_lib_handle, "interpret");
	hashmap_set("bf", cmd_bf, bot->commands);
}

void destroy(struct bot *bot) {
	hashmap_remove("bf", bot->commands);
}
