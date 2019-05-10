#include "stdlib.h"
#include "string.h"
#include "skiString.h"
#include "regex.h"

char* initBuffer(char* str, unsigned int size)
{
	PBuffer buffer = NULL;

	if(str){
		buffer = (PBuffer)(str - sizeof(TBuffer));
		if(buffer->size < size){
			size = BUFFER_SIZE(size);
			if((buffer = realloc(buffer, size + sizeof(TBuffer))) == NULL){
				goto BUFFER_REALLOC_FAILED;
			}

			buffer->size = size;
			str = buffer->buf;
		}
	}else{
		size = BUFFER_SIZE(size);
		if((buffer = malloc(size + sizeof(TBuffer))) == NULL){
			goto BUFFER_MALLOC_FAILED;
		}

		buffer->size = size;
		str = buffer->buf;
	}

	return str;

BUFFER_REALLOC_FAILED:
BUFFER_MALLOC_FAILED:
	return NULL;
}

unsigned int measureBuffer(char* str)
{
	PBuffer buffer = (PBuffer)(str - sizeof(TBuffer));

	return buffer->size;
}

int freeBuffer(char* str)
{
	PBuffer buffer = (PBuffer)(str - sizeof(TBuffer));

	if(buffer->size < STR_BLK_SIZE || buffer->size%STR_BLK_SIZE != 0){
		return -1;
	}

	free(buffer);
	return 0;
}

PStrWrapper regString(char* buf, char* regstr)
{
	regex_t reg = {0};
	PStrWrapper strw = NULL;
	//regmatch_t pmatch[12] = {0};

	if(regcomp(&reg, regstr, REG_NEWLINE)){
		return NULL;
		//goto REG_COMPILE_FAILED;
	}

	regmatch_t pmatch[reg.re_nsub + 1];
	if(regexec(&reg, buf, reg.re_nsub + 1, pmatch, 0)){
		goto REG_EXECUTE_FAILED;
	}

	if((strw = malloc(sizeof(TStrWrapper) + sizeof(char*) * (reg.re_nsub+1)*2)) == NULL){
		goto WRAPPER_MALLOC_FAILED;
	}

	memset(strw, 0, sizeof(TStrWrapper));

	strw->buf = buf;
	strw->sep = regstr;
	strw->num = (reg.re_nsub+1) * 2;

	int i, j;
	for(i = 0, j = 0; i < reg.re_nsub + 1; i++){
		strw->field[j++] = buf + pmatch[i].rm_so;
		strw->field[j++] = buf + pmatch[i].rm_eo;
	}

	regfree(&reg);
	return strw;

	free(strw);
	strw = NULL;
WRAPPER_MALLOC_FAILED:
REG_EXECUTE_FAILED:
	regfree(&reg);
REG_COMPILE_FAILED:
	return NULL;
}

PStrWrapper wrapString(char* buf, char* sep)
{
	PStrWrapper strw = NULL;
	char* sbuf = NULL;
	char* scur = NULL;
	int sepLen = 0;
	int snum = 0;
	int size = 0;

	size = strlen(buf) + 1; 
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
		if(strw->buf && strw->size)
			free(strw->buf);
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

char* lowerString(char* str)
{
	char* cur = str;
	while(*cur){
		if(*cur > 0x40 && *cur < 0x5b){
			*cur += 0x20;
		}
		cur++;
	}
	return str;
}

char* upperString(char* str)
{
	char* cur = str;
	while(*cur){
		if(*cur > 0x60 && *cur < 0x7b){
			*cur -= 0x20;
		}
		cur++;
	}
	return str;
}

