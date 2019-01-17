#ifndef __SKIHASH_H__
#define	__SKIHASH_H__

#include "utypes.h"

typedef struct _node{
	char *key;
	struct _node *next;
}TMapNode, *PMapNode;

#define MAP_SIZE_GENE (3/4)
#define MAP_SIZE_DEFAULT (64)

PMapNode createHashMap(uint32 size);
PMapNode adjustHashMap();
PMapNode searchHashMap(char *key);
PMapNode eraseHashMap(char *key);
PMapNode insertHashMap(char *key, PMapNode pNode);
#endif //skiHash.h
