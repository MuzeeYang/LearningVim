#ifndef __SKIHASH_H__
#define	__SKIHASH_H__

#include "utypes.h"

typedef struct _node{
	char *key;
	struct _node *next;
}TMapNode, *PMapNode;

typedef struct _hashHead{
	uint32 size;
	uint32 length;
	PMapNode hashmapPtr;
}TMapHead, *PMapHead;

#define MAP_SIZE_GENE (0.75)
#define MAP_SIZE_DEFAULT (64)

PMapHead createHashMap(uint32 size);
int freeHashMap(PMapHead pHead);

PMapNode searchHashMap(PMapHead, char*);
PMapNode eraseHashMap(PMapHead, char*);
PMapNode insertHashMap(PMapHead, PMapNode, char*);
#endif //skiHash.h
