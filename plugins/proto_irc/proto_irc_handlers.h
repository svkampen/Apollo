#ifndef _PROTO_IRC_HANDLERS_H_
#define _PROTO_IRC_HANDLERS_H_
#include "bot.h"
#include "message.h"

void irc_chan_join(struct bot *bot, struct message *msg);
void irc_privmsg(struct bot *bot, struct message *msg);
void irc_invite(struct bot *bot, struct message *msg);

#endif
