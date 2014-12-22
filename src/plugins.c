#include "plugins.h"
#include "main.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int load_plugin(struct bot *bot, char *name) {
	char *filename = calloc(1, 14+strlen(name));
	strcat(filename, "./plugins/");
	strcat(filename, name);
	strcat(filename, ".so");
	
	void (*init)(struct bot*);

	void *plug = dlopen(filename, RTLD_NOW);

	if (!plug) {
		fprintf(stderr, "[core\twarn] error loading plugin: %s\n", dlerror());
		free(filename);
		return 0;
	}

	init = (void (*)(struct bot*))dlsym(plug, "init");
	if (!init) {
		fprintf(stderr, "[core\twarn] error loading plugin init: %s\n", dlerror());
		free(filename);
		dlclose(plug);
		return 0;
	}

	init(bot);

	hashmap_set(name, plug, bot->plugins);
	printf("[core\tinfo] loaded plugin %s\n", name);

	free(filename);
	return 1;
}

void unload_plugin(void *val) {
	void (*destr)(struct bot*) = (void (*)(struct bot*))dlsym(val, "destroy");
	if (!destr) {
		fprintf(stderr, "[core\twarn] could not load destroy function: %s\n", dlerror());
	} else {
		destr(&b);
	}

	dlclose(val);
}

void __unload_plugin(char *key, void *val) {
	printf("[core\tinfo] unloading plugin %s\n", key);

	unload_plugin(val);
}

