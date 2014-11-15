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
void *__asm__result___;

#define doasm(CODE, INPUT, CLOBBERED_REGS) __asm__(CODE \
												:"=r"(__asm__result___) \
												:INPUT \
												:CLOBBERED_REGS), \
										   __asm__result___
						

void destroy(struct bot *bot) {
	fclose(fp);
}

void init (struct bot *bot) {
	fp = fdopen(dup(bot->socket), "w");
int i; printf("%p\n", &i); fflush(stdout);
}