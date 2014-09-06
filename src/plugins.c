#include "plugins.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int load_plugin(struct bot *bot, char *name) {
	char *filename = calloc(1, 14+strlen(name));
	strcat(filename, "./plugins/");
	strcat(filename, name);
	strcat(filename, ".so");
	
	printf("%s\n", filename);

	void (*init)(struct bot*);

	void *plug = dlopen(filename, RTLD_LAZY);

	if (plug == NULL) {
		fprintf(stderr, "[core] error loading plugin: %s\n", dlerror());
		free(filename);
		return 0;
	}

	init = (void (*)(struct bot*))dlsym(plug, "init");
	if (init == NULL) {
		fprintf(stderr, "[core] error loading plugin init: %s\n", dlerror());
		free(filename);
		return 0;
	}

	init(bot);

	free(filename);
	return 1;
}

	
