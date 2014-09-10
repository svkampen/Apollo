#include "hashmap.h"
#include "bot.h"
#include "message.h"
#include "net.h"
#include <stdlib.h>
#include <string.h>
#include "unistd.h"


int runproc(struct bot *bot, char *chan) {
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
		execl("/bin/bash", "/bin/bash", "-c", "cd ../plugins; python3 createplug.py", (char*) NULL);
	}

	close(pipefd[1]);
	output = fdopen(pipefd[0], "r");

	while (fgets(line, 1024, output)) {
		char *msg = strchr(line, ' ')+1;
		if (startswith(msg, "note") || startswith(msg, "In function ‘init’")) {
			continue;
		}
		bot->msg(bot, chan, strchr(line, ' ')+1);
	}

	waitpid(pid, &status, 0);
	fclose(output);
	return status;
}




void run_dmc(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->msg(bot, chan, "usage: run <c code>");
		bot->msg(bot, chan, "runs c code in current process :0");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		bot->msg(bot, chan, "\x01" "ACTION giggles" "\x01");
		return;
	}


	FILE *f1 = fopen("../plugins/.tmp.c", "r");
	FILE *f2 = fopen("../plugins/.plug.c", "w");

	char line[1024];

	while (fgets(line, 1024, f1) != NULL) {
		fprintf(f2, "%s", line);
	}

	fprintf(f2, "%s\n", args);
	fprintf(f2, "}");

	fclose(f1);
	fclose(f2);

	int code = runproc(bot, chan);

	if (code != 0) {
		return;
	}

	load_plugin(bot, "plug");
	unload_plugin(hashmap_get("plug", bot->plugins));
	hashmap_remove("plug", bot->plugins);
}

void init(struct bot *bot) {
	hashmap_set("run", run_dmc, bot->commands);
}

void destroy(struct bot *bot) {
	return;
}
