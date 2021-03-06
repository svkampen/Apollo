#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>

#include "net.h"
#include "main.h"
#include "bot.h"
#include "list.h"
#include "hashmap.h"
#include "message.h"
#include "plugins.h"

#include "cJSON.h"
#include "config.h"

#define EQ(a, b) (strcmp(a, b) == 0)
#define CJSON_GET_STRING(var, name) char *name = cJSON_GetObjectItem(b.config, name)->valuestring;

typedef struct double_link ll_link;
typedef void (*handler_t)(struct bot*, struct message*);

void __msg(struct bot*, char*, char*);

int main(int argc, char *argv[]) {
	if(access("config.json", F_OK)) {
		// no config file.
		printf("[core\tfail] no config file (config.json) found, exiting..\n");
		exit(-1);
	}

	b.admin = "svkampen";

	load_config(&b);
	load_protocol(&b);

	init_handlers();
	b.proto->init(&b);

	b.proto->connect();
	b.proto->tick();
	b.proto->destroy();
	destroy();

	dlclose(b.proto->protolib);

	printf("[core\tinfo] bye!\n");
	return 0;
}

void destroy() {
	hashmap_foreach(b.plugins, __unload_plugin);

	hashmap_destroy(b.plugins);
	hashmap_destroy(b.handlers);
	hashmap_destroy(b.commands);

	for (int i = 0; i < b.channels->index; ++i) {
		free(b.channels->list[i]);
	}
	VEC_DESTROY(b.channels);
	cJSON_Delete(b.config);
}

void init_handlers() {
	b.channels = calloc(1, sizeof(b.channels));
	VEC_INIT(b.channels, 4);
	b.plugins = hashmap_create(4);
	b.handlers = hashmap_create(8);
	b.commands = hashmap_create(8);

	b.tick_functions = calloc(1, sizeof(struct double_link));

	if (!load_plugin(&b, "common")) {
		fprintf(stderr, "[core\tfail] error loading common plugin, exiting..\n");
	}

	if (!load_plugin(&b, "uncommon")) {
		printf("[core\tinfo] the uncommon plugin is not present.\n");
	}
}
