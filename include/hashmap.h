#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#include "list.h"
#include <inttypes.h>

struct hashmap {
	struct double_link **arr;
	int length;
	int size;
};

typedef struct {
	uint64_t hash;
	char *key;
	void *value;
} hashed_pair;

void *hashmap_get(char *key, struct hashmap *ht);
void hashmap_set(char *key, void *ptr, struct hashmap *ht);
struct hashmap *hashmap_create(int size);
void hashmap_destroy(struct hashmap *hm);
void hashmap_resize(struct hashmap *ht);
#endif
