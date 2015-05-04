#include "net.h"
#include "bot.h"
#include "list.h"
#include "message.h"
#include "plugins.h"
#include "hashmap.h"
#include "protocol.h"
#include "httpapi.h"
#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

typedef struct http_response httpresp;

static FILE *fp;
void *__asm__result___;

#define _(...) bot->proto->msg("#programming", __VA_ARGS__);

typedef int __;

#define doasm(CODE, INPUT, CLOBBERED_REGS) __asm__(CODE \
												:"=r"(__asm__result___) \
												:INPUT \
												:CLOBBERED_REGS), \
										   __asm__result___
#ifdef __cplusplus
extern "C"
#endif
void destroy(struct bot *bot) {
	fclose(fp);
}


#ifdef __cplusplus
extern "C"
#endif
void init (struct bot *bot) {
	fp = fdopen(dup(bot->socket), "w");
