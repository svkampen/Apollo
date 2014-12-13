#include "config.h"
#include "util.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_config(struct bot *bot) {
	char *file = load_file("config.json");

	if (!file) {
		exit(-1);
	}

	cJSON *root = cJSON_Parse(file);

	cJSON *host = cJSON_GetObjectItem(root, "host");
	cJSON *port = cJSON_GetObjectItem(root, "port");
	cJSON *nick = cJSON_GetObjectItem(root, "nick");

	if (!host || !port || !nick) {
		fputs("[core\tfail] incomplete config file; missing host, nick, or port!\n", stderr);
		exit(-1);
	}

	bot->host = host->valuestring; bot->port = port->valuestring; bot->nick = nick->valuestring;
	bot->config = root;

	free(file);
}
