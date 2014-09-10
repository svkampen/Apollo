#include "net.h"
#include "bot.h"
#include "handlers.h"
#include "list.h"
#include "message.h"
#include "parse.h"
#include "plugins.h"
#include "hashmap.h"

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>

static FILE *fp;

void destroy(struct bot *bot) {
	fclose(fp);
}

void init (struct bot *bot) {
	fp = fdopen(dup(bot->socket), "w");
sockprintf(bot->socket, "PRIVMSG #programming :%d\r\n", SCHAR_MAX);
}