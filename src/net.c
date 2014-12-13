#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "net.h"

// #include <debug.h>

void sendall(int sockfd, const char *msg) {
	int sent;
	int len = strlen(msg);

	sent = send(sockfd, msg, len, 0);
	while (sent != len) {
		sent += send(sockfd, msg+sent, len-sent, 0);
	}
}

// get proper sockaddr, ipv4 or ipv6
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// returns an addrinfo linked list. needs to be freed
// with freeaddrinfo later!
struct addrinfo *get_addr_info(const char *host, const char *port) {
	struct addrinfo hints, *res;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(host, port, &hints, &res)) != 0) {
		fprintf(stderr, "[net] getaddrinfo failed: %s\n", gai_strerror(status));
		return NULL;
	}

	return res;
}

int getsock(struct addrinfo *info) {
	struct addrinfo *p;
	int sockfd;

	char ipstr[INET6_ADDRSTRLEN];

	for (p = info; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "[net] client failed to connect.\n");
		exit(8);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			ipstr, sizeof ipstr);
	printf("[net\tinfo] connecting to %s...\n", ipstr);
	printf("[net\tinfo] connected.\n");

	return sockfd;
}

void sockprintf(int sockfd, const char *fmt, ...) {
	va_list a;
	va_start(a, fmt);

	vssockprintf(sockfd, fmt, a);

	va_end(a);
}

void vssockprintf(int sockfd, const char *fmt, va_list a) {
	va_list b;
	va_copy(b, a);
	int len = vsnprintf(NULL, 0, fmt, b);

	char *buf = (char*) calloc(len + 3, 1);

	vsnprintf(buf, len+1, fmt, a);

	char *end = buf + len;
	sprintf(end, "%s", "\r\n");

#ifdef DEBUG
	printf("<<< %s\n", buf);
#endif

	sendall(sockfd, buf);
	free(buf);
	va_end(b);
}
