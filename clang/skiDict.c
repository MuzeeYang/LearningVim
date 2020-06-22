#include "stdlib.h"
#include "string.h"
#include "skiDict.h"


static int _freeValueByNode(PMapNode pNode)
{
	PDict dictory = NULL;
	if(pNode){
		dictory = GET_DICT_BY_KEY(pNode);
		if(dictory->key_node.key)free(dictory->key_node.key);
		//if(TYPE_PTR == dictory->type)free(dictory->data.v);
		if(TYPE_STR == dictory->type)free(dictory->data.s.ptr);
		free(dictory);
	}
	return 0;
}

void* createDict(unsigned int size)
{
	return createHashMap(size);
}

void freeValueDict(PDict dictory)
{
	if(dictory)_freeValueByNode(&dictory->key_node);
}

void freeDict(void* handle)
{
	freeHashMap(handle, _freeValueByNode);
}

PDict pushValueDict(DICT_TYPE type, void* value, unsigned int vlen)
{
	PDict dictory = NULL;//malloc(sizeof(TDict));

	if((dictory = malloc(sizeof(TDict))) == NULL)goto dict_mem_failed;
	
	memset(dictory, 0, sizeof(TDict));
	//dictory->key_node.key = strdup(key);
	dictory->type = type;
	switch(type){
		case TYPE_NUM:
		case TYPE_DOUBLE:
			if(vlen <= sizeof(dictory->data))memcpy(&dictory->data.l, value, vlen);
			break;
		case TYPE_STR:
			if((dictory->data.s.ptr = malloc(vlen)) == NULL)goto dict_vmem_failed;
			memcpy(dictory->data.s.ptr, value, vlen);
			dictory->data.s.len = vlen;
			break;
		default:
			goto dict_type_failed;
	}

	return dictory;

dict_type_failed:
	if(TYPE_STR == dictory->type)free(dictory->data.s.ptr);
dict_vmem_failed:
	free(dictory);
dict_mem_failed:
	return NULL;
}

int pushDict(void* handle, char* key, PDict dictory)
{
	if(dictory == NULL || handle == NULL)goto dict_arg_failed;

	dictory->key_node.key = strdup(key);
	if(dictory->key_node.key == NULL)goto dict_key_failed;

	_freeValueByNode(insertHashMap(handle, &dictory->key_node));
	return 0;

	free(dictory->key_node.key);
dict_key_failed:
dict_arg_failed:
	return -1;
}

PDict popDict(void* handle, char* key)
{
	PMapNode pNode = NULL;//searchHashMap(handle, key);

	if(handle == NULL)return 0;
	pNode = eraseHashMap(handle, key);
	if(pNode == NULL)return 0;

	return GET_DICT_BY_KEY(pNode);
}




