#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *load_file(char *filename) {
	FILE *f = fopen(filename, "rb");

	if (!f) {
		fprintf(stderr, "[core\tfail] Error opening file %s", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *str = malloc(fsize + 1);
	size_t read_bytes = fread(str, 1, fsize, f);
	if (read_bytes != fsize) {
		fprintf(stderr, "[core\tfail] failed to read all bytes from file %s", filename);
		fprintf(stderr, "             read %lu bytes out of %ld", read_bytes, fsize);
		return NULL;
	}

	fclose(f);

	str[fsize] = 0;
	return str;
}

