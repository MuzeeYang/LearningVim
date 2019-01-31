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

#define MAP_SIZE_GENE (3/4)
#define MAP_SIZE_DEFAULT (64)

void createHashMap(PMapHead, uint32);
PMapNode searchHashMap(PMapHead, char *);
PMapNode eraseHashMap(PMapHead, char *);
PMapNode insertHashMap(PMapHead, PMapNode);
#endif //skiHash.h
