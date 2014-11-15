#ifndef _NET_H_
#define _NET_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdarg.h>
#include <netinet/in.h>

void sendall(int sockfd, const char *msg);
void *get_in_addr(struct sockaddr *sa);
struct addrinfo *get_addr_info(const char *host, const char *port);
int getsock(struct addrinfo *info);
void sockprintf(int sockfd, const char *fmt, ...);
void vssockprintf(int sockfd, const char *fmt, va_list a);

#endif
