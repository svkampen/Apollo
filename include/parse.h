#ifndef _PARSE_H_
#define _PARSE_H_
#include "message.h"

struct message *parse(char *data); 
int startswith(const char *str, const char *pre);
char *irc_getline(char *buf);
char *irc_nextline();
void freemsg(struct message *msg);
char *last(char *data);
#endif
