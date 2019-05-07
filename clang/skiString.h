#ifndef __SKISTRING_H__
#define __SKISTRING_H__

#define STR_BLK_SIZE (0x400)

typedef struct _strWrapper{
	char* buf;
	int size;
	char* sep;
	int num;
	char* field[0];
}TStrWrapper, *PStrWrapper;

PStrWrapper wrapString(char* buf, int size, char* sep);
int freeString(PStrWrapper strw);
char* joinString(char** field, int size, char* sep);

#endif //skiString.h
