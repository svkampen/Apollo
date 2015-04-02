#include "hashmap.h"
#include "plugins.h"
#include "bot.h"
#include "message.h"
#include "net.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int status_fifo = 0;

static int isfifo(char *filename) {
	struct stat st;
	stat(filename, &st);
	return S_ISFIFO(st.st_mode);
}

void fifo_tick(struct bot *bot) {
	status_fifo = open("status", O_WRONLY | O_NDELAY);
	if (status_fifo != -1) {
		dprintf(status_fifo, "Status: running, obviously. How else would you be able to read this FIFO ;)\n");
		close(status_fifo);
	}
	status_fifo = 0;
}

void init(struct bot *bot) {
	mkfifo("status", 0644);
	if (!isfifo("status")) {
		printf("[fifo\tfail] Error initializing status FIFO: %s\n", strerror(errno));
	} else {
		push_val(bot->tick_functions, fifo_tick);
	}
}

void destroy(struct bot *bot) {
	struct double_link *i, *next;
	for (struct double_link *i = bot->tick_functions; i != NULL; i = next) {
		next = i->next;
		if (i->ptr == fifo_tick) {
			remove_link(i);
		}
	}
}


