#include <netinet/in.h> // INET6_ADDRSTRLEN
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
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

	{
		struct addrinfo *info = get_addr_info(b.host, b.port);

		b.socket = getsock(info);
		b.running = 1;

		// we won't need this anymore.
		freeaddrinfo(info);
	}

	b.proto->init(&b);
	init_handlers();

	b.proto->connect();
	run();
	destroy();

	close(b.socket);
	dlclose(b.proto->protolib);

	printf("[core\tinfo] bye!\n");
	return 0;
}

void destroy() {
	hashmap_foreach(b.plugins, __unload_plugin);

	hashmap_destroy(b.plugins);
	cJSON_Delete(b.config);
}

void init_handlers() {
	b.plugins = hashmap_create(4);

	if (!load_plugin(&b, "common")) {
		fprintf(stderr, "[core\tfail] error loading common plugin, exiting..\n");
	}
	
	if (!load_plugin(&b, "uncommon")) {
		printf("[core\tinfo] the uncommon plugin is not present.\n");
	}
}

void run() {
	int nbytes;
	char data[BUFSIZE/2];

	while (b.running) {
		memset(&data, 0, BUFSIZE/2);

		recv:
		if ((nbytes = recv(b.socket, data, (BUFSIZE/2)-1, 0)) == -1) {
			if (errno == EINTR) {
				// what a pain in the ass,
				// we got interrupted >.>
				goto recv;
			}

			perror("recv failed");
			exit(4);
		}

		strcat(b.buffer, data);

		if (nbytes == 0) {
			b.running = 0;
		}

		b.proto->tick();
	}
}
			
void __msg(struct bot *bot, char *chan, char *msg) {
	sockprintf(bot->socket, "PRIVMSG %s :%s", chan, msg);
}
