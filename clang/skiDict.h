#ifndef __SKIDICT_H__
#define __SKIDICT_H__

#include "skiHash.h"

typedef enum _type{
	TYPE_NUM = 1,
	TYPE_DOUBLE,
	TYPE_STR,
}DICT_TYPE;

typedef union _data{
	unsigned char c;
	unsigned int n;
	unsigned long long l;
	double f;
	struct {unsigned int len; void* ptr;} s;
}DICT_DATA;

typedef struct _dict{
	DICT_TYPE type;
	DICT_DATA data;
	TMapNode key_node;
}TDict, *PDict;

#define DICT_OFFSET(base) ((unsigned long)(&((PDict)(base))->key_node)) 
#define GET_DICT_BY_KEY(addr) ((PDict)((unsigned long)(addr) - DICT_OFFSET(0)))

#define popValueDict_char(pdict) ((char)(pdict)->data.c)
#define popValueDict_int(pdict) ((int)(pdict)->data.n)
#define popValueDict_long(pdict) ((long long)(pdict)->data.l)
#define popValueDict_uchar(pdict) ((unsigned char)(pdict)->data.c)
#define popValueDict_uint(pdict) ((unsigned int)(pdict)->data.n)
#define popValueDict_ulong(pdict) ((unsigned long long)(pdict)->data.l)
#define popValueDict_double(pdict) ((double)(pdict)->data.f)
#define popValueDict_struct(pdict) ((void*)(pdict)->data.s.ptr)
#define popValueDict_length(pdict) ((unsigned int)(pdict)->data.s.len)
#define popValueDict_type(pdict) ((DICT_TYPE)(pdict)->type)

void* createDict(unsigned int size);
void freeValueDict(PDict dictory);
void freeDict(void* handle);
PDict pushValueDict(DICT_TYPE type, void* value, unsigned int vlen);
int pushDict(void* handle, char* key, PDict dictory);
PDict popDict(void* handle, char* key);
//unsigned long long popValueDict(PDict dictory);
#endif //skiDict.h
