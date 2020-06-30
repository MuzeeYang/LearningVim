#ifndef __SKIDICT_H__
#define __SKIDICT_H__

#include "skiHash.h"

typedef enum _type{
	TYPE_NULL,
	TYPE_NUMBER = 1,
	TYPE_DOUBLE,
	TYPE_BOOLEN,
	TYPE_STRING,
	TYPE_STRUCT,
	TYPE_ARRAY,
	TYPE_DICT,
}DICT_OBJ_TYPE;

typedef union _data{
	unsigned char c;
	unsigned int n;
	unsigned long long l;
	double f;
	struct {unsigned int len; void* ptr;} s;
}DICT_OBJ_DATA;

typedef struct _dict_obj{
	DICT_OBJ_TYPE type;
	DICT_OBJ_DATA data;
	TMapNode key_node;
}TDictObj, *PDictObj;

#define DICT_OFFSET(base) ((unsigned long)(&((PDictObj)(base))->key_node)) 
#define GET_DICT_BY_KEY(addr) ((PDictObj)((unsigned long)(addr) - DICT_OFFSET(0)))

#define popValueDict_char(pobj) ((char)(pobj)->data.c)
#define popValueDict_int(pobj) ((int)(pobj)->data.n)
#define popValueDict_long(pobj) ((long long)(pobj)->data.l)
#define popValueDict_uchar(pobj) ((unsigned char)(pobj)->data.c)
#define popValueDict_uint(pobj) ((unsigned int)(pobj)->data.n)
#define popValueDict_ulong(pobj) ((unsigned long long)(pobj)->data.l)
#define popValueDict_double(pobj) ((double)(pobj)->data.f)
#define popValueDict_string(pobj) ((char*)(pobj)->data.s.ptr)
#define popValueDict_struct(pobj) ((void*)(pobj)->data.s.ptr)
#define popValueDict_bool(pobj) popValueDict_uchar(pobj)
#define popValueDict_array(pobj) ((PDictObj)(pobj)->data.s.ptr)
#define popValueDict_dict(pobj) popValueDict_struct(pobj)

#define popValueDict_length(pobj) ((unsigned int)(pobj)->data.s.len)
#define popValueDict_type(pobj) ((DICT_OBJ_TYPE)(pobj)->type)

void* createDict(unsigned int size);
void freeValueDict(PDictObj object);
void freeDict(void* handle);
PDictObj pushValueDict(DICT_OBJ_TYPE type, void* value, unsigned int vlen);
int pushDict(void* handle, char* key, PDictObj object);
PDictObj popDict(void* handle, char* key);


#define DICT_JSON_ENABLE
#ifdef DICT_JSON_ENABLE

#include "skiString.h"

char* wrapJsonDict(void* handle);
void* parseJsonDict(char* buf);
#endif

//unsigned long long popValueDict(PDictObj object);
#endif //skiDict.h
