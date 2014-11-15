#include "protocol.h"
#include <string.h>
#include <dlfcn.h>

void load_protocol(struct bot *bot) {
	cJSON *protocol_node = cJSON_GetObjectItem(bot->config, "protocol");

	if (!protocol_node) {
		fputs("[core\tfail] incomplete config file; missing protocol!\n", stderr);
		exit(-1);
	}

	char *protocol = protocol_node->valuestring;
	char *prefix = "plugins/proto_";
	char *filename = calloc(strlen(prefix) + strlen(protocol) + 4, 1);
	strcat(filename, prefix);
	strcat(filename, protocol);
	strcat(filename, ".so");

	void *protolib = dlopen(filename, RTLD_NOW);

	if (!protolib) {
		fprintf(stderr, "[core\tfail] error loading library for protocol %s: %s\n", protocol, dlerror());
		exit(-1);
	}

	struct apollo_protocol *proto;
	proto = (struct apollo_protocol*)dlsym(protolib, "proto");
	proto->protolib = protolib;

	if (!proto) {
		fprintf(stderr, "[core\tfail] error loading protocol struct from %s protocol: %s\n", protocol, dlerror());
		exit(-1);
	}

	free(filename);
	bot->proto = proto;
}
