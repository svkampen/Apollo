#ifndef _HANDLERS_H_
#define _HANDLERS_H_
#include "bot.h"
#include "parse.h"

#define EQ(a, b) (strcmp(a, b) == 0)

void privmsg(struct bot *b, struct message *msg);

#endif
