#include "stdlib.h"
#include "string.h"
#include "skiHash.h"

static int __strCmp(char *str1, char *str2)
{
	if(str1 == str2)
		return 0;
	else
		return strcmp(str1, str2);
}

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
	for(i = seed; i > 2 && i+1 != j; i--)
		for(j = 2; j < i && i % j; j++);

	return i+1;
}

void __adjustHashMap(PMapHead pHead)
{
	PMapNode tmp;
	uint32 newSize = __getHashMapSize(pHead->size*2);
	if(newSize > pHead->size){
		tmp = realloc(pHead->hashmapPtr, sizeof(TMapNode)*newSize);
		if(tmp){
			pHead->hashmapPtr = tmp;
			memset(pHead->hashmapPtr + pHead->size, 0, newSize - pHead->size);
			pHead->size = newSize;
		}
	}
}

void createHashMap(PMapHead pHead, uint32 size)
{
	if(size)
		pHead->size = __getHashMapSize(size/MAP_SIZE_GENE);
	else
		pHead->size = __getHashMapSize(MAP_SIZE_DEFAULT);

	pHead->hashmapPtr = malloc(sizeof(TMapNode) * pHead->size);
	memset(pHead->hashmapPtr, 0, sizeof(TMapNode) * pHead->size);
	pHead->length = 0;
}

PMapNode searchHashMap(PMapHead pHead, char *key)
{
	uint32 index = __bkdrHash(key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr[index];

	while(cursor->next && __strCmp(cursor->next->key, key))cursor = cursor->next;

	return cursor->next;
}

PMapNode eraseHashMap(PMapHead pHead, char *key)
{
	uint32 index = __bkdrHash(key) % pHead->size;
	PMapNode cursor = pHead->gHashMapPtr[index];
	PMapNode tmp;

	while(cursor->next && __strCmp(cursor->next->key, key))cursor = cursor->next;
	tmp = cursor->next;

	if(tmp){
		cursor->next = tmp->next;
		tmp->next = NULL;
		gHashMapNums--;
	}

	return tmp;
}

PMapNode insertHashMap(PMapHead pHead, PMapNode pNode)
{
	if(pHead->length+1 > pHead->size*MAP_SIZE_GENE)
		__adjustHashMap(pHead);

	uint32 index = __bkdrHash(pNode->key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr[index];
	PMapNode tmp;

	while(cursor->next && __strCmp(cursor->next->key, key))cursor = cursor->next;
	tmp = cursor->next;

	if(tmp){
		cursor->next = pNode;
		pNode->next = tmp->next;
		tmp->next = NULL;
	}else{
		cursor->next = pNode;
		pNode->next = NULL;
		pHead->length++;
	}

	return tmp;
}

