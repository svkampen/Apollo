#ifndef _PARSE_H_
#define _PARSE_H_
#include "message.h"
#include "bot.h"

struct message *parse(char *data); 
int startswith(const char *str, const char *pre);
char *irc_getline(struct bot *b);
char *irc_nextline(struct bot *b);
void freemsg(struct message *msg);
char *last(char *data);
#endif
