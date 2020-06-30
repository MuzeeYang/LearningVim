#ifndef __SKISTRING_H__
#define __SKISTRING_H__

#define BUFFER_BLK_SIZE (0xff)
#define BUFFER_GET_BFID(addr) ((((unsigned long)(addr) >> 8) & BUFFER_BLK_SIZE) ^ ((unsigned long)(addr) & BUFFER_BLK_SIZE))
#define BUFFER_GET_SZID(sz) ((sz) & BUFFER_BLK_SIZE)
#define BUFFER_GET_SZ(sz) ((sz) & ~BUFFER_BLK_SIZE)


struct _buffer{
	unsigned long len;
	char buf[0];
};

typedef struct _strWrapper{
	char* buf;
	int size;
	char* sep;
	int num;
	char* field[0];
}TStrWrapper, *PStrWrapper;

void* initBuffer(void* str, unsigned int size);
void freeBuffer(void* str);

PStrWrapper wrapRegex(char* buf, char* regstr);
PStrWrapper wrapString(char* buf, char* sep);
PStrWrapper findString(char* str, char* need);
int freeWrapper(PStrWrapper strw);

char* regString(char* buf, char* regstr);
char* joinString(char** field, int size, char* sep); //need to freeBuffer
char* lowerString(char* str);
char* upperString(char* str);
char* skipString(char* str);
char* washString(char* str);
char* pathString(char* root, char* file); //need to freeBuffer
char* cutString(char* str, int start, int end);	//need to freeBuffer
char* spliceString(char* str, int start, int cutlen, char* buf);
int intString(char* str);
double floatString(char *str);

#endif //skiString.h
