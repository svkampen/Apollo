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

#include "net.h"
#include "parse.h"
#include "bot.h"
#include "list.h"
#include "hashmap.h"
#include "handlers.h"

#define EQ(a, b) (strcmp(a, b) == 0)

typedef struct double_link ll_link;
typedef void (*handler_t)(struct bot*, struct message*);

static char NICK[] = "apollo";
static char HOST[] = "irc.sublumin.al";
static char PORT[] = "6667";


static struct bot b;

void init_handlers();
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

	run();
	close(b.socket);

	printf("[net] bye!\n");

	return 0;
}

void init_handlers() {
	b.handlers = hashmap_create(32);
	struct double_link *privmsg_handlers = calloc(1, sizeof(struct double_link));
	hashmap_set("PRIVMSG", (void*)privmsg_handlers, b.handlers);

	privmsg_handlers->ptr = (void*)privmsg;
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

		for (char *i = irc_getline(b.buffer); i != NULL; i = irc_nextline(&b)) {

#ifdef DEBUG
			printf("%s\n", i);
#endif

			if (startswith(i, "PING")) {
				char *t = last(i);
				sockprintf(b.socket, "PONG %s", t);
				free(t);
				continue;
			}

			struct message *msg = parse(i);

			if (EQ(msg->meth, "422") || EQ(msg->meth, "376")) {
				// welcome message
				// join #programming
				printf("[core] endofmotd received.\n");
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
			

