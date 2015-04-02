#include "hashmap.h"
#include "bot.h"
#include "message.h"
#include "net.h"
#include "plugins.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "util.h"
#include "math.h"

#include <sys/mman.h>

#define BOOL(x) (!(!(x)))

#define SET_BIT(arg, pos) ((arg) | (1L << (pos)))
#define TEST_BIT(arg, pos) BOOL((arg) & (1L << (pos)))

long double number = 0;

void lshift_double(struct bot *bot, char *nick, char *chan, char *args) {
	int shift = 0;
	if (args == NULL)
		shift = 1;
	else
		shift = atoi(args);
	__uint128_t *as_int = &number;
	*as_int <<= shift;
}

void put_n(struct bot *bot, char *chan) {
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void square_n(struct bot *bot, char *nick, char *chan, char *args) {
	number *= number;
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void double_n(struct bot *bot, char *nick, char *chan, char *args) {
	number += number;
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void sqrt_n(struct bot *bot, char *nick, char *chan, char *args) {
	number = sqrt(number);
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void inc_n(struct bot *bot, char *nick, char *chan, char *args) {
	number++;
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void dec_n(struct bot *bot, char *nick, char *chan, char *args) {
	number--;
	bot->proto->msg(chan, "The number is now %Lg, you bastard", number);
}

void lshift(struct bot *bot, char *nick, char *chan, char *args) {
	lshift_double(bot, nick, chan, args);
	put_n(bot, chan);
}

void rshift(struct bot *bot, char *nick, char *chan, char *args) {
	int shift = 0;
	if (args == NULL)
		shift = 1;
	else
		shift = atoi(args);
	__uint128_t *as_int = (__uint128_t)&number;
	*as_int >>= shift;
	put_n(bot, chan);
}


void init(struct bot *bot) {
	/* int bytes[8];
	for (int i = 0; i < 64; i += 8) {
		bytes[i] = (long)&nullcall & (0xFF00000000000000 >> i);
	}

	char tocopy[] = {0x01, 0xb8, 0x00, 0x00, 0x48, 0x00, bytes[7], 0xbb, bytes[5], bytes[6],
		bytes[3], bytes[4], bytes[1], bytes[2], 0x48, bytes[0], 0x03, 0x89, 0x00, 0xc3}; */

	/* mmap(0, 4096, PROT_READ | PROT_WRITE | PROT_EXEC,
			MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
	memset((char*)NULL, '\xc3', 1); */
	hashmap_set("square", square_n, bot->commands);
	hashmap_set("sqrt", sqrt_n, bot->commands);
	hashmap_set("inc", inc_n, bot->commands);
	hashmap_set("dec", dec_n, bot->commands);
	hashmap_set("lshift", lshift, bot->commands);
	hashmap_set("rshift", rshift, bot->commands);
}

void destroy(struct bot *bot) {
	hashmap_remove("square", bot->commands);
	hashmap_remove("sqrt", bot->commands);
	hashmap_remove("inc", bot->commands);
	hashmap_remove("dec", bot->commands);
	hashmap_remove("lshift", bot->commands);
	hashmap_remove("rshift", bot->commands);
}
