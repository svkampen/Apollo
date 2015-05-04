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

#include <sys/mman.h>
#include <stdio.h>

int nullcall = 0;

int startswith(const char *str, const char *pre) {
	size_t lenpre = strlen(pre),
		   lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

int runproc(struct bot *bot, char *chan, uint8_t is_cpp) {
	pid_t pid = 0;
	int pipefd[2];
	FILE *output;
	char line[1024];

	int status;
	int firstline = 1;

	pipe(pipefd);
	pid = fork();

	if (pid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		if (is_cpp) {
			execl("/bin/bash", "/bin/bash", "-c", "cd ../plugins; python3 createplug.py cpp", (char*) NULL);
		} else {
			execl("/bin/bash", "/bin/bash", "-c", "cd ../plugins; python3 createplug.py", (char*) NULL);
		}
	}

	close(pipefd[1]);
	output = fdopen(pipefd[0], "r");

	while (fgets(line, 1024, output)) {
		char *msg = strchr(line, ' ')+1;
		if (startswith(msg, "note") || startswith(msg, "In function ‘init’")) {
			continue;
		}
		bot->proto->msg(chan, strchr(line, ' ')+1);
	}

	waitpid(pid, &status, 0);
	fclose(output);
	return status;
}

void run_dmc(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: run [-p] <c code>");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		bot->proto->msg(chan, "\x01" "ACTION giggles" "\x01");
		return;
	}

	uint8_t is_cpp = 0;
	system("cp ../plugins/.tmp.c ../plugins/.plug.c");
	FILE *f2 = fopen("../plugins/.plug.c", "a");

	if (startswith(args, "-p")) {
		fprintf(f2, "%s", strchr(args, ' ')+1);
		is_cpp = 1;
	} else {
		fprintf(f2, "%s", args);
	}

	if (!(args[strlen(args)-1] == ';')) {
		fprintf(f2, ";");
	}
	fprintf(f2, "\n}");

	fclose(f2);

	int code = runproc(bot, chan, is_cpp);

	if (code != 0) {
		return;
	}


	load_plugin(bot, "plug");
	unload_plugin(hashmap_get("plug", bot->plugins));
	hashmap_remove("plug", bot->plugins);
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
	hashmap_set("run", run_dmc, bot->commands);
}

void destroy(struct bot *bot) {
	hashmap_remove("run", bot->commands);
}
