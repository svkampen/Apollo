#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *nextline;
char *parseline;

int startswith(const char *str, const char *pre) {
	size_t lenpre = strlen(pre),
		   lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

char *irc_getline(char *buf) {
	char *line = strdup(strtok_r(buf, "\r\n", &nextline));
	return line;
}

char *irc_nextline() {
	char *line = strtok_r(NULL, "\r\n", &nextline);
	if (line == NULL) {
		return NULL;
	}

	return strdup(line);
}

char *last(char *data) {
	char *part = strtok(data, " ");
	char *toret;

	while ((part = strtok(NULL, " ")) != NULL) {
		toret = part;
	}

	toret = strdup(toret);
	return toret;
}

struct message *parse(char *data) {
	struct message *m = calloc(1, sizeof (struct message));

	if (startswith(data, "PING")) {
		m->meth = strdup("PING");
	    m->arg = last(data);
	} else {
		m->host = strdup(strtok_r(data, " ", &parseline));
		m->meth = strdup(strtok_r(NULL, " ", &parseline));
		m->arg = strdup(strtok_r(NULL, "", &parseline));
	}

	parseline = NULL;

	return m;
}

void freemsg(struct message *msg) {
	free(msg->host);
	free(msg->meth);
	free(msg->arg);
	free(msg);
}
