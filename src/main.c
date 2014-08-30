#include "net.h"
#include "parse.h"
#include "bot.h"
#include "list.h"
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

#define EQ(a, b) (strcmp(a, b) == 0)

typedef struct double_link link;
typedef void (*)(struct bot*, struct message*) handler_t;

static char NICK[] = "apollo";
static char HOST[] = "irc.sublumin.al";
static char PORT[] = "6667";


static struct bot b;
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

	sockprintf(b.socket, "NICK %s", NICK);
	sockprintf(b.socket, "USER apollo * * :hi im artemis' sis");

	run();
	close(b.socket);

	printf("[net] bye!\n");

	return 0;
}

link *get_handlers(char *type) {
	link *handlist = NULL;
	for (link *i = handlers; i != NULL; i = i->next) {
		void **list = (void**)i->value;
		if (EQ(((char*)list[0]), type)) {
			handlist = (link*)list[1];
			break;
		}
	}
	return handlist;
}

void run() {
	int nbytes;

	while (b.running) {
		memset(&b.buffer, 0, BUFSIZE);

		recv:
		if ((nbytes = recv(b.socket, b.buffer, BUFSIZE-1, 0)) == -1) {
			if (errno == EINTR) {
				// what a pain in the ass,
				// we got interrupted >.>
				goto recv;
			}

			perror("recv failed");
			exit(4);
		}

		if (nbytes == 0) {
			b.running = 0;
		}

		for (char *i = irc_getline(b.buffer); i != NULL; i = irc_nextline()) {
#ifdef DEBUG
			printf("%s\n", i);
#endif
			if (startswith(i, "PING")) {
				char *t = last(i);
				sockprintf(b.socket, "PONG %s", t);
				free(t);
			}

			struct message *msg = parse(i);

			if (EQ(msg->meth, "422") || EQ(msg->meth, "376")) {
				// welcome message
				// join #programming
				printf("[core] endofmotd received.\n");
				sockprintf(b.socket, "JOIN #bots");
			}

			if (EQ(msg->meth, "PRIVMSG")) {
				// yeah yeah hack because I haven't implemented
				// handlers yet.
				link *hand = (link*)gethandlers("PRIVMSG");
				for(link *i = hand; i != NULL; i = i->next) {
					((handler_t)i->value)(&b, msg);
				}
			}
			

			freemsg(msg);
			free(i);
		}
	}
}
			

