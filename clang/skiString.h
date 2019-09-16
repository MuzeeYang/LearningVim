#ifndef __SKISTRING_H__
#define __SKISTRING_H__

#define STR_BLK_SIZE (0x100)
#define BUFFER_BLOCK(size) (((size)&~0xff) + (((size)&0xff) != 0)*0x100)
#define BUFFER_SIZE(buf) (*((unsigned long*)(buf) - 1) & ~0x3)
//#define BUFFER_SIZE(size) ((size)/STR_BLK_SIZE*STR_BLK_SIZE + ((size)%STR_BLK_SIZE != 0)*STR_BLK_SIZE)

typedef struct _strWrapper{
	char* buf;
	int size;
	char* sep;
	int num;
	char* field[0];
}TStrWrapper, *PStrWrapper;

char* initBuffer(char* str, unsigned int size);

PStrWrapper wrapRegex(char* buf, char* regstr);
PStrWrapper wrapString(char* buf, char* sep);
int freeWrapper(PStrWrapper strw);

char* regString(char* buf, char* regstr);
char* joinString(char** field, int size, char* sep); //need to free
char* lowerString(char* str);
char* upperString(char* str);
char* skipString(char* str);
char* washString(char* str);
char* pathString(char* root, char* file); //need to free

#endif //skiString.h
