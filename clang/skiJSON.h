#ifndef __SKIJSON_H__
#define __SKIJSON_H__

typedef struct _object{
	struct _object* next;
	int type;
	char* name;
	union{
		char boolen;
		double number;
		char* string;
		struct _object* child;
	}value;
}TObject, *PObject;

enum _objType{
	OBJECT_TYPE_NULL,
	OBJECT_TYPE_BOOL,
	OBJECT_TYPE_NUMBER,
	OBJECT_TYPE_STRING,
	OBJECT_TYPE_ARRAY,
	OBJECT_TYPE_OBJECT
};


PObject createObject();
int freeObject(PObject pobj);
char* wrapJSON(PObject pobj);
PObject parseJSON(char* buffer);

#endif //skiJSON.h
