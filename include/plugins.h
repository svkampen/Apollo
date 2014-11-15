#ifndef _PLUGINS_H_
#define _PLUGINS_H_

#include "bot.h"

int load_plugin(struct bot *bot, char *name);
void __unload_plugin(char *key, void *val);
void unload_plugin(void *val);
#endif
