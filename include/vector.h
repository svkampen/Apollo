#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <stdlib.h>
#include <stdio.h>

#define VEC_NEEDS_RESIZE(vec) (((vec)->index + 1) == (vec)->size)

#define VEC_PUSH(vec, item) \
    if (VEC_NEEDS_RESIZE(vec)) { \
		printf("Resizing vector at %p\n", (vec)); \
		(vec)->size *= 2; \
		__typeof__((vec)->list) tmp = (__typeof__((vec)->list))realloc((vec)->list, (vec)->size * sizeof(item)); \
		if (!tmp) { \
			printf("Error resizing vector!\n"); \
			exit(-1); \
		} \
		(vec)->list = tmp; \
	} \
	\
	(vec)->list[(vec)->index++] = item

#define VEC_INIT(vec, items) \
	(vec)->list = calloc(items, sizeof(__typeof__(*((vec)->list)))); \
	(vec)->size = items; \
	(vec)->index = 0

#define VEC_DESTROY(vec) \
	free((vec)->list); \
	(vec)->list = NULL; \
	(vec)->index = (vec)->size = 0;

#endif
