#ifndef _APOLLO_PROTO_IRC_PARSE_H_
#define _APOLLO_PROTO_IRC_PARSE_H_
#include "bot.h"
#include "message.h"

struct message *parse(char *data); 
int startswith(const char *str, const char *pre);
char *irc_getline(struct bot *b);
char *irc_nextline(struct bot *b);
void freemsg(struct message *msg);
char *last(char *data);
#endif
