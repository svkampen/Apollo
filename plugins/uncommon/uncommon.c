#include "hashmap.h"
#include "bot.h"
#include "message.h"
#include "net.h"
#include "plugins.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


// sshhh
#include <signal.h>
#include <setjmp.h>

jmp_buf *jmp;

int startswith(const char *str, const char *pre) {
	size_t lenpre = strlen(pre),
		   lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

static void handler(int sig) {
	if (sig == SIGSEGV) {
		longjmp(*jmp, 1);
	}
}

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
		bot->proto->msg(chan, strchr(line, ' ')+1);
	}

	waitpid(pid, &status, 0);
	fclose(output);
	return status;
}

void run_dmc(struct bot *bot, char *nick, char *chan, char *args) {
	if (!args) {
		bot->proto->msg(chan, "usage: run <c code>");
		bot->proto->msg(chan, "runs c code in current process :0");
		return;
	}

	if (strcmp(nick, bot->admin) != 0) {
		bot->proto->msg(chan, "\x01" "ACTION giggles" "\x01");
		return;
	}


	FILE *f1 = fopen("../plugins/.tmp.c", "r");
	FILE *f2 = fopen("../plugins/.plug.c", "w");
	char line[1024];
	jmp_buf jump;
	jmp = &jump;

	while (fgets(line, 1024, f1) != NULL) {
		fprintf(f2, "%s", line);
	}

	fprintf(f2, "%s", args);
	if (!args[strlen(args)-1] == ';') {
		fprintf(f2, ";");
	}
	fprintf(f2, "\n}");

	fclose(f1);
	fclose(f2);

	int code = runproc(bot, chan);

	if (code != 0) {
		return;
	}


	if (!setjmp(jump)) {
		load_plugin(bot, "plug");
		unload_plugin(hashmap_get("plug", bot->plugins));
		hashmap_remove("plug", bot->plugins);
	} else {
		signal(SIGSEGV, handler);
		bot->proto->msg(chan, "You think someone would do that? Raise SIGSEGV? On the INTERNET?");
	}
		
}

void init(struct bot *bot) {
	hashmap_set("run", run_dmc, bot->commands);
	signal(SIGSEGV, handler);
}

void destroy(struct bot *bot) {
	hashmap_remove("run", bot->commands);
	signal(SIGSEGV, SIG_DFL);
}
