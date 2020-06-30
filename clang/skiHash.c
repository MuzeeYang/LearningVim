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

static unsigned int _bkdrHash(char *str)
{
	unsigned int hash = 0;
	unsigned int seed = 31;//31 131 1313 13131...
	while(*str){
		hash = hash*seed + *str++;
	}
	return hash;
}

static unsigned int _getHashMapSize(unsigned int seed)
{
	unsigned int i, j;
	for(i = seed; i > 2 && i+1 != j; i--)
		for(j = 2; j < i && i % j; j++);

	return i+1;
}

static PMapNode _rmMapNode(PMapNode cursor)
{
	PMapNode pNode = NULL;
	pNode = cursor->next;
	cursor->next = pNode->next;
	pNode->next = NULL;
	return pNode;
}

static void _mkMapNode(PMapNode cursor, PMapNode pNode)
{
	while(cursor->next)cursor = cursor->next;
	cursor->next = pNode;
	pNode->next = NULL;
}

static PMapNode _locateHashMap(PMapHead pHead, char *key)
{
	unsigned int index = _bkdrHash(key) % pHead->size;
	PMapNode cursor = pHead->hashmapPtr + index;

	//if(!_strCmp(key, "50"))printf("%s: size = %d, index = %d, hashNo = %u\n", __func__, pHead->size, index, _bkdrHash(key));

	while(cursor->next && _strCmp(cursor->next->key, key))cursor = cursor->next;

	return cursor;
}

static void _adjustMapScale(PMapNode pMap, unsigned int orgSize, unsigned int modSize)
{
	int i;
	PMapNode cursor;
	unsigned int hashNo;

	for(i = 0; i < orgSize; i++){
		cursor = pMap + i;
		while(cursor->next){
			hashNo = _bkdrHash(cursor->next->key);
			if(i != hashNo % modSize)
				_mkMapNode(pMap + (hashNo%modSize), _rmMapNode(cursor));
			else
				cursor = cursor->next;
		}
	}
}

static void _adjustHashMap(PMapHead pHead)
{
	PMapNode tmp;
	unsigned int newSize = _getHashMapSize(pHead->size*2);
	if(newSize > pHead->size){
		tmp = realloc(pHead->hashmapPtr, sizeof(TMapNode)*newSize);
		if(tmp){
			pHead->hashmapPtr = tmp;
			memset(pHead->hashmapPtr + pHead->size, 0, sizeof(TMapNode) * (newSize-pHead->size));
			_adjustMapScale(pHead->hashmapPtr, pHead->size, newSize);
			pHead->size = newSize;
		}
	}
}

PMapHead createHashMap(unsigned int size)
{
	PMapHead pHead;
	if(size)
		size = _getHashMapSize(size * (1 + MAP_SIZE_GENE));
	else
		size = _getHashMapSize(MAP_SIZE_DEFAULT);

	if((pHead = malloc(sizeof(TMapHead))) == NULL)
		goto HEAD_MALLOC_FAILED;

	pHead->size = size;

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

int freeHashMap(PMapHead pHead, MAP_FREENODE_FUNC func)
{
	PMapNode pNode = NULL;
	PMapNode cursor = NULL;
	int i;

	if(pHead){
		if(pHead->hashmapPtr){
			for(i = 0; i < pHead->size; i++){
				//printf("nhead = %d:\n", i);
				cursor = pHead->hashmapPtr + i;
				while(cursor->next){
					pNode = _rmMapNode(cursor);
					if(func)func(pNode);
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
	PMapNode cursor = NULL;
	if(key)cursor = _locateHashMap(pHead, key);
	else return NULL;

	return cursor->next;
}

PMapNode eraseHashMap(PMapHead pHead, char* key)
{
	PMapNode cursor = NULL;//_locateHashMap(pHead, key);
	PMapNode ret = NULL;

	if(key)cursor = _locateHashMap(pHead, key);
	else return NULL;

	if(cursor->next){
		ret = _rmMapNode(cursor);
		pHead->length--;
	}

	return ret;
}

PMapNode insertHashMap(PMapHead pHead, PMapNode pNode)
{
	if(pHead->length > pHead->size*MAP_SIZE_GENE)
		_adjustHashMap(pHead);

	PMapNode cursor = NULL;//_locateHashMap(pHead, pNode->key);
	PMapNode ret = NULL;

	if(pNode && pNode->key)cursor = _locateHashMap(pHead, pNode->key);
	else return NULL;

	if(cursor->next)ret = _rmMapNode(cursor);
	else pHead->length++;

	_mkMapNode(cursor, pNode);

	return ret;
}

int keysHashMap(PMapHead pHead, char* keys[])
{
	int i = 0;
	PMapNode cursor;
	if(pHead == NULL)return -1;
	if(keys == NULL)return pHead->length;

	for(i = 0; i < pHead->size; i++){
		cursor = pHead->hashmapPtr + i;
		while(cursor->next){
			if(keys)*keys++ = cursor->next->key;//keys++;}
			cursor = cursor->next;
		}
	}
	return pHead->length;
}

