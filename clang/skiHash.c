#include "stdlib.h"
#include "string.h"
#include "skiHash.h"

static int _strCmp(char *str1, char *str2)
{
	if(str1 == str2)
		return 0;
	else
		return strcmp(str1, str2);
}

static uint32 _bkdrHash(char *str)
{
	uint32 hash = 0;
	uint32 seed = 31;//31 131 1313 13131...
	while(*str){
		hash = hash*seed + *str++;
	}
	return hash;
}

static uint32 _getHashMapSize(uint32 seed)
{
	uint32 i, j;
	for(i = seed; i > 2 && i+1 != j; i--)
		for(j = 2; j < i && i % j; j++);

	return i+1;
}

static void _adjustHashMap(PMapHead pHead)
{
	PMapNode tmp;
	uint32 newSize = _getHashMapSize(pHead->size*2);
	if(newSize > pHead->size){
		tmp = realloc(pHead->hashmapPtr, sizeof(TMapNode)*newSize);
		if(tmp){
			pHead->hashmapPtr = tmp;
			memset(pHead->hashmapPtr + pHead->size, 0, sizeof(TMapNode)*(newSize - pHead->size));
			pHead->size = newSize;
		}
	}
}

PMapHead createHashMap(uint32 size)
{
	PMapHead pHead;
	if((pHead = malloc(sizeof(TMapHead))) == NULL)
		goto HEAD_MALLOC_FAILED;

	if(size)
		pHead->size = _getHashMapSize(size/MAP_SIZE_GENE);
	else
		pHead->size = _getHashMapSize(MAP_SIZE_DEFAULT);

	if((pHead->hashmapPtr = malloc(sizeof(TMapNode) * pHead->size)) == NULL)
		goto MAP_MALLOC_FAILED;

	memset(pHead->hashmapPtr, 0, sizeof(TMapNode) * pHead->size);
	pHead->length = 0;

	return pHead;

	free(pHead->hashmapPtr);
MAP_MALLOC_FAILED:
	free(pHead);
HEAD_MALLOC_FAILED:
	return NULL;
}

int freeHashMap(PMapHead pHead)
{
	PMapNode pNode = NULL;
	if(pHead){
		if(pHead->hashmapPtr){
			int i;
			for(i = 0; i < pHead->size; i++){
				while(pHead->hashmapPtr[i].next){
					pNode = pHead->hashmapPtr[i].next;
					pHead->hashmapPtr[i].next = pNode->next;
					free(pNode);
				}
			}
			free(pHead->hashmapPtr);
		}
		free(pHead);
	}
	return 0;
}

PMapNode searchHashMap(PMapHead pHead, char *key)
{
	uint32 index = _bkdrHash(key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr + index;

	while(cursor->next && _strCmp(cursor->next->key, key))cursor = cursor->next;

	return cursor->next;
}

PMapNode eraseHashMap(PMapHead pHead, char* key)
{
	uint32 index = _bkdrHash(key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr + index;
	PMapNode tmp;

	while(cursor->next && _strCmp(cursor->next->key, key))cursor = cursor->next;
	tmp = cursor->next;

	if(tmp){
		cursor->next = tmp->next;
		tmp->next = NULL;
		pHead->length--;
	}

	return tmp;
}

PMapNode insertHashMap(PMapHead pHead, PMapNode pNode, char* key)
{
	if(pHead->length+1 > pHead->size*MAP_SIZE_GENE)
		_adjustHashMap(pHead);

	if(key)pNode->key = key;
	uint32 index = _bkdrHash(pNode->key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr + index;
	PMapNode tmp;

	while(cursor->next && _strCmp(cursor->next->key, pNode->key))cursor = cursor->next;
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

