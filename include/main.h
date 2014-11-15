#ifndef _APOLLO_MAIN_H_
#define _APOLLO_MAIN_H_
#include "bot.h"

char *load_file(char *filename);
void load_config();
void load_protocol();
void destroy();
void init_handlers();
void run();

struct bot b;

#endif
