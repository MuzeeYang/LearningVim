#ifndef __SKISTRING_H__
#define __SKISTRING_H__

#define STR_BLK_SIZE (0x400)
#define BUFFER_SIZE(size) (((size)&~(STR_BLK_SIZE-1)) + (((size)&(STR_BLK_SIZE-1)) != 0)*STR_BLK_SIZE)
//#define BUFFER_SIZE(size) ((size)/STR_BLK_SIZE*STR_BLK_SIZE + ((size)%STR_BLK_SIZE != 0)*STR_BLK_SIZE)

typedef struct _strBuffer{
	unsigned int size;
	char buf[0];
}TBuffer, *PBuffer;

typedef struct _strWrapper{
	char* buf;
	int size;
	char* sep;
	int num;
	char* field[0];
}TStrWrapper, *PStrWrapper;

char* initBuffer(char* str, unsigned int size);
unsigned int measureBuffer(char* str);
int freeBuffer(char* str);

PStrWrapper regString(char* buf, char* regstr);
PStrWrapper wrapString(char* buf, char* sep);
int freeString(PStrWrapper strw);
char* joinString(char** field, int size, char* sep);
char* lowerString(char* str);
char* upperString(char* str);

#endif //skiString.h
