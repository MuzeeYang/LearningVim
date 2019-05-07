#include "stdlib.h"
#include "string.h"

PStrWrapper wrapString(char* buf, int size, char* sep)
{
	PStrWrapper strw = NULL;
	char* sbuf = NULL;
	char* scur = NULL;
	int sepLen = 0;
	int snum = 0;

	if((sbuf = malloc(size)) == NULL){
		goto BUFFER_MALLOC_FAILED;
	}
	//memset(sbuf, 0, size);
	memcpy(sbuf, buf, size);
 
	sepLen = strlen(sep);
	scur = sbuf;
	while(sepLen && (scur = strstr(scur, sep))){
		memset(scur, 0, sepLen);
		scur += sepLen;
		snum++;
	}
	snum++;

	if((strw = malloc(sizeof(TStrWrapper) + snum*sizeof(char*))) == NULL){
		goto WRAPPER_MALLOC_FAILED;
	}

	int i;
	for(i = 0, scur = sbuf; i < snum; i++){
		strw->field[i] = scur;
		scur += strlen(scur) + sepLen;
	}

	strw->buf = sbuf;
	strw->size = size;
	strw->sep = sep;
	strw->num = snum;

	return strw;

	free(strw);
	strw = NULL;
WRAPPER_MALLOC_FAILED:
	free(sbuf);
	sbuf = NULL;
BUFFER_MALLOC_FAILED:
	return NULL;
}

int freeString(PStrWrapper strw)
{
	if(strw){
		free(strw->buf);
		//free(strw->sep);
		free(strw);
	}
	return 0;
}

char* joinString(char** field, int size, char* sep)
{
	char* str = NULL;
	char* cur = NULL;
	int strBlk = 0;
	int sepLen = 0;
	int fieldLen = 0;
	int strLen = 0;

	if((str = malloc(STR_BLK_SIZE * ++strBlk)) == NULL){
		goto BUFFER_MALLOC_FAILED;
	}

	sepLen = strlen(sep);
	int i;
	for(i = 0; i < size; i++){

		fieldLen = strlen(field[i]);
		if(strLen + fieldLen + sepLen + 1 > STR_BLK_SIZE * strBlk){
			if((cur = realloc(str, STR_BLK_SIZE * ++strBlk)) == NULL){
				goto BUFFER_REALLOC_FAILED;
			}
			str = cur;
		}

		strcpy(str + strLen, field[i]);
		strLen += fieldLen;

		if(i+1 < size){
			strcpy(str + strLen, sep);
			strLen += sepLen;
		}
	}

	return str;

BUFFER_REALLOC_FAILED:
	free(str);
BUFFER_MALLOC_FAILED:
	return NULL;
}

