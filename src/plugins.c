#include "plugins.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int load_plugin(struct bot *bot, char *name) {
	char *filename = calloc(1, 14+strlen(name));
	strcat(filename, "./plugins/");
	strcat(filename, name);
	strcat(filename, ".so");
	
	void (*init)(struct bot*);

	void *plug = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);

	if (!plug) {
		fprintf(stderr, "[core\twarn] error loading plugin: %s\n", dlerror());
		free(filename);
		return 0;
	}

	init = (void (*)(struct bot*))dlsym(plug, "init");
	if (!init) {
		fprintf(stderr, "[core\twarn] error loading plugin init: %s\n", dlerror());
		free(filename);
		return 0;
	}

	init(bot);

	hashmap_set(name, plug, bot->plugins);
	printf("[core\tinfo] loaded plugin %s\n", name);

	free(filename);
	return 1;
}

	
