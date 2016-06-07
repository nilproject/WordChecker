#pragma once

#include <stdlib.h>
#include <stdbool.h>

#if _WIN32 || _WIN64
typedef ptrdiff_t ssize_t;
#endif

typedef struct { const _Bool fake; } HashSet;

HashSet *hashSet_create(void);

void hashSet_free(HashSet *const ptTree, const _Bool fFreeKeys);

_Bool hashSet_insert(HashSet *const pHashSet, char *const sKey, const _Bool fCopyKey);

_Bool hashSet_contains(const HashSet *const pHashSet, const char *const sKey);

size_t hashSet_count(const HashSet *ptTree);
