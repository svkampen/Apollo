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

char *irc_getline(struct bot *b) {
	if (!strstr(b->buffer, "\r\n")) {
		return NULL;
	}
	char *line = strdup(strtok_r(b->buffer, "\r\n", &nextline));
	return line;
}

char *irc_nextline(struct bot *b) {
	if ((strcmp(nextline+1, "") != 0) && !strstr(nextline+1, "\r\n")) {
		// act like nothing happened and just return NULL :P
		char *rest = strdup(nextline);
		memset(&b->buffer, 0, BUFSIZE);
		strcpy(b->buffer, rest);
		free(rest);
		return NULL;
	}

	char *line = strtok_r(NULL, "\r\n", &nextline);
	if (line == NULL) {
		memset(&b->buffer, 0, BUFSIZE);
		return NULL;
	}

	return strdup(line);
}

char *last(char *data) {
	char *part = strtok(data, " ");
	char *toret = NULL;

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
		m->host = strtok_r(data, " ", &parseline);

		if (m->host != NULL) {
			m->host = strdup(m->host);
		}

		m->meth = strtok_r(NULL, " ", &parseline);

		if (m->meth != NULL) {
			m->meth = strdup(m->meth);
		}

		m->arg = strtok_r(NULL, "", &parseline);
		if (m->arg != NULL) {
			m->arg = strdup(m->arg);
		}
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
