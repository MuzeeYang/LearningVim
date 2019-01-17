#include "stdlib.h"
#include "string.h"
#include "skiHash.h"

static uint32 gHashMapSize;
static uint32 gHashMapNums;
static PMapNode gHashMapPtr;

static uint32 __bkdrHash(char *str)
{
	uint32 hash = 0;
	uint32 seed = 31;//31 131 1313 13131...
	while(*str){
		hash = hash*seed + *str++;
	}
	return hash;
}

static uint32 __getHashMapSize(uint32 seed)
{
	uint32 i, j;
	for(i = seed*2; i > 2 && i+1 != j; i--)
		for(j = 2; j < i && i % j; j++);

	return i+1;
}

PMapNode createHashMap(uint32 size)
{
	if(size)
		gHashMapSize = __getHashMapSize(size/MAP_SIZE_GENE);
	else
		gHashMapSize = __getHashMapSize(MAP_SIZE_DEFAULT);

	gHashMapPtr = malloc(sizeof(TMapNode) * gHashMapSize);
	memset(gHashMapPtr, 0, sizeof(TMapNode) * gHashMapSize);
	gHashMapNums = 0;
	return gHashMapPtr;
}

PMapNode adjustHashMap()
{
	PMapNode tmp;
	uint32 newSize = __getHashMapSize(gHashMapSize);
	if(newSize > gHashMapSize){
		tmp = realloc(gHashMapPtr, sizeof(TMapNode)*newSize);
		if(tmp){
			gHashMapPtr = tmp;
			memset(gHashMapPtr+gHashMapSize, 0, newSize-gHashMapSize);
			gHashMapSize = newSize;
		}
	}
	return gHashMapPtr;
}

PMapNode searchHashMap(char *key)
{
	uint32 index = __bkdrHash(key) % gHashMapSize;
	PMapNode cursor = gHashMapPtr[index];

	while(cursor->next && strcmp(cursor->next->key, key))cursor = cursor->next;

	return cursor->next;
}

PMapNode eraseHashMap(char *key)
{
	uint32 index = __bkdrHash(pNode->key) % gHashMapSize;
	PMapNode cursor = gHashMapPtr[index];
	PMapNode tmp;

	while(cursor->next && strcmp(cursor->next->key, key))cursor = cursor->next;
	tmp = cursor->next;

	if(tmp){
		cursor->next = tmp->next;
		tmp->next = NULL;
		gHashMapNums--;
	}

	return tmp;
}

PMapNode insertHashMap(char *key, PMapNode pNode)
{
	if(gHashMapNums+1 > gHashMapSize*MAP_SIZE_GENE)
		adjustHashMap();

	uint32 index = __bkdrHash(pNode->key) % gHashMapSize;
	PMapNode cursor = gHashMapPtr[index];
	pNode->key = key;

	while(cursor->next && strcmp(cursor->next->key, key))cursor = cursor->next;
	tmp = cursor->next;

	if(tmp){
		cursor->next = pNode;
		pNode->next = tmp->next;
		tmp->next = NULL;
	}else{
		cursor->next = pNode;
		pNode->next = NULL;
		gHashMapNums++;
	}

	return tmp;
}

