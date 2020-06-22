#ifndef __SKIHASH_H__
#define	__SKIHASH_H__

typedef struct _node{
	char *key;
	struct _node *next;
}TMapNode, *PMapNode;

typedef struct _hashHead{
	unsigned int size;
	unsigned int length;
	PMapNode hashmapPtr;
}TMapHead, *PMapHead;

#define MAP_SIZE_GENE (0.75)
#define MAP_SIZE_DEFAULT (64)

typedef int (*MAP_FREENODE_FUNC)(PMapNode);

PMapHead createHashMap(unsigned int size);
int freeHashMap(PMapHead pHead, MAP_FREENODE_FUNC func);

PMapNode searchHashMap(PMapHead, char*);
PMapNode eraseHashMap(PMapHead, char*);
PMapNode insertHashMap(PMapHead, PMapNode);
int keysHashMap(PMapHead pHead, char* keys[]);


#endif //skiHash.h
