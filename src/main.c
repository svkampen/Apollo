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
#include "parse.h"
#include "bot.h"
#include "list.h"
#include "hashmap.h"
#include "handlers.h"
#include "plugins.h"

#define EQ(a, b) (strcmp(a, b) == 0)
#ifdef _GNUC_
#define lambda(ret_type, name, args, body) \
	({ \
	 ret_type name args \
	 body \
	 &name; \
	 })
#endif

typedef struct double_link ll_link;
typedef void (*handler_t)(struct bot*, struct message*);

static char NICK[] = "apollo";
static char HOST[] = "irc.sublumin.al";
static char PORT[] = "6667";


static struct bot b;

void init_handlers();
void destroy();
void __msg(struct bot*, char*, char*);
void run();

int main(int argc, char *argv[]) {
	{
		struct addrinfo *info;
		if (argc > 1) {
			info = get_addr_info(argv[1], PORT);
		} else {
			info = get_addr_info(HOST, PORT);
		}

		b.socket = getsock(info);
		b.running = 1;

		// we won't need this anymore.
		freeaddrinfo(info);
	}

	init_handlers();

	sockprintf(b.socket, "NICK %s", NICK);
	sockprintf(b.socket, "USER apollo * * :hi im artemis' sis");

	b.admin = "svkampen";

	run();
	destroy();
	close(b.socket);

	hashmap_destroy(b.handlers);
	hashmap_destroy(b.commands);
	hashmap_destroy(b.plugins);


	printf("[core\tinfo] bye!\n");

	return 0;
}

void unload_plugin(void *val) {
	void (*destr)(struct bot*) = (void (*)(struct bot*))dlsym(val, "destroy");
	if (!destr) {
		fprintf(stderr, "[core\twarn] could not load destroy function: %s\n", dlerror());
		fprintf(stderr, "[core\twarn] THIS IS A POSSIBLE MEMORY LEAK!.\n");
	} else {
		destr(&b);
	}

	dlclose(val);
}

void __unload_plugin(char *key, void *val) {
	printf("[core\tinfo] unloading plugin %s\n", key);

	unload_plugin(val);
}

void destroy() {
	hashmap_foreach(b.plugins, __unload_plugin);

	struct double_link *ph = hashmap_get("PRIVMSG", b.handlers);
	if (ph) {
		free_list(ph);
	}
}


void init_handlers() {

	b.msg = __msg;

	b.handlers = hashmap_create(16);
	struct double_link *privmsg_handlers = calloc(1, sizeof(struct double_link));
	hashmap_set("PRIVMSG", (void*)privmsg_handlers, b.handlers);

	privmsg_handlers->ptr = (void*)privmsg;

	b.commands = hashmap_create(16);
	b.plugins = hashmap_create(16);
	b.data = hashmap_create(8);

	if (!load_plugin(&b, "common")) {
		fprintf(stderr, "[core\tfail] error loading common plugin, exiting..\n");
		exit(8);
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

		char *i = irc_getline(&b);
		if (i == NULL) {
			continue;
		}

		for (; i != NULL; i = irc_nextline(&b)) {

#ifdef DEBUG
			printf("%s\n", i);
#endif

			if (startswith(i, "PING")) {
				char *t = last(i);
				sockprintf(b.socket, "PONG %s", t);
				free(t);
				free(i);
				continue;
			}

			struct message *msg = parse(i);

			if (EQ(msg->meth, "422") || EQ(msg->meth, "376")) {
				// welcome message
				// join #programming
				sockprintf(b.socket, "JOIN #bots");
			}

			ll_link *handlers = hashmap_get(msg->meth, b.handlers);
			for (ll_link *i = handlers; i != NULL; i = i->next) {
				((handler_t)i->ptr)(&b, msg);
			}

			freemsg(msg);
			free(i);
		}
	}
}
			
void __msg(struct bot *bot, char *chan, char *msg) {
	sockprintf(bot->socket, "PRIVMSG %s :%s", chan, msg);
}
