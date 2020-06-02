//#include "stdlib.h"
#include "malloc.h"
#include "string.h"
#include "skiString.h"
#include "regex.h"

char* initBuffer(char* str, unsigned int newSize)
{
	if(str){
		unsigned int oldSize = BUFFER_SIZE(str);
		if(oldSize < newSize){
			//newSize = BUFFER_BLOCK(newSize);
			if((str = realloc(str, newSize)) == NULL){
				goto BUFFER_REALLOC_FAILED;
			}
			newSize = BUFFER_SIZE(str);

			memset(str + oldSize, 0, newSize - oldSize);
		}
	}else{
		//newSize = BUFFER_BLOCK(newSize);
		if((str = malloc(newSize)) == NULL){
			goto BUFFER_MALLOC_FAILED;
		}
		newSize = BUFFER_SIZE(str);

		memset(str, 0, newSize);
	}

	return str;

BUFFER_REALLOC_FAILED:
BUFFER_MALLOC_FAILED:
	return NULL;
}

char* regString(char* buf, char* regstr)
{
	char* ret = NULL;
	regex_t reg = {0};
	//PStrWrapper strw = NULL;
	regmatch_t tmatch = {0, 0};

	if(regcomp(&reg, regstr, REG_NEWLINE)){
		goto REG_COMPILE_FAILED;
	}

	if(regexec(&reg, buf, 1, &tmatch, 0)){
		goto REG_EXECUTE_FAILED;
	}

	ret = buf + tmatch.rm_so;

REG_EXECUTE_FAILED:
	regfree(&reg);
REG_COMPILE_FAILED:
	return ret;
}

PStrWrapper wrapRegex(char* buf, char* regstr)
{
	regex_t reg = {0};
	PStrWrapper strw = NULL;
	regmatch_t* pmatch = NULL;
	char* cur = NULL;
	int size = 0;
	int strLen = 0;

	if(regcomp(&reg, regstr, REG_NEWLINE)){
		goto REG_COMPILE_FAILED;
	}

	if((pmatch = malloc((reg.re_nsub+1)*sizeof(regmatch_t))) == NULL){
		goto PMATCH_MALLOC_FAILED;
	}

	if(regexec(&reg, buf, reg.re_nsub + 1, pmatch, 0)){
		goto REG_EXECUTE_FAILED;
	}

	if((strw = malloc(sizeof(TStrWrapper) + sizeof(char*)*(reg.re_nsub+1))) == NULL){
		goto WRAPPER_MALLOC_FAILED;
	}

	//size = strlen(buf) + 1;
	if((strw->buf = initBuffer(NULL, 0)) == NULL){
		goto BUFFER_MALLOC_FAILED;
	}

	strw->sep = regstr;
	strw->num = reg.re_nsub+1;
	int i;
	for(i = 0, size = 0, strLen = 0; i < strw->num; i++){
		size = pmatch[i].rm_eo - pmatch[i].rm_so;

		if((cur = initBuffer(strw->buf, strLen + size + 1)) == NULL){
			goto BUFFER_REALLOC_FAILED;
		}
		strw->buf = cur;
		//strw->size = size;

		memcpy(strw->buf + strLen, buf + pmatch[i].rm_so, size);
		strw->field[i] = strw->buf + strLen;
		strLen += size;
		strw->buf[strLen++] = 0;
	}

	strw->size = BUFFER_SIZE(strw->buf);

	free(pmatch);
	regfree(&reg);
	return strw;

BUFFER_REALLOC_FAILED:
	free(strw->buf);
BUFFER_MALLOC_FAILED:
	free(strw);
WRAPPER_MALLOC_FAILED:
REG_EXECUTE_FAILED:
	free(pmatch);
PMATCH_MALLOC_FAILED:
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

int freeWrapper(PStrWrapper strw)
{
	if(strw){
		if(strw->buf && strw->size)free(strw->buf);
		free(strw);
	}
	return 0;
}

char* joinString(char** field, int size, char* sep)
{
	char* str = NULL;
	char* cur = NULL;
	int sepLen = 0;
	int fieldLen = 0;
	int strLen = 0;

	if((str = initBuffer(str, strLen)) == NULL){
		goto BUFFER_MALLOC_FAILED;
	}

	sepLen = strlen(sep);
	int i;
	for(i = 0; i < size; i++){
		fieldLen = strlen(field[i]);

		if((cur = initBuffer(str, strLen + fieldLen + sepLen + 1)) == NULL){
			goto BUFFER_REALLOC_FAILED;
		}
		str = cur;

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

char* skipString(char* str)
{
	while(str && *str && *str <= 0x20)str++;
	return str;
}

char* washString(char* str)
{
	if(str == NULL)return NULL;
	char* start = str;
	char* end = str + strlen(str) - 1;

	while(*start && *start <= 0x20)start++;
	while(*end && *end <= 0x20)end--;

	if(start > end)return NULL;

	end[1] = 0;
	return start;
}

char* pathString(char* root, char* file)
{
	root = washString(root);
	file = washString(file);
	if(root == NULL && file == NULL)return NULL;
	int length = 0;
	char* str = NULL;

	if(root)length += strlen(root);
	if(file)length += strlen(file);

	str = initBuffer(NULL, length + 3);
	if(str == NULL)return NULL;

	length = 0;
	if(root){
		while(*root){
			if(root[0] == '/' && root[1] == '/'){root++; continue;}
			if(root[0] == '\\' && root[1] == '\\'){root++; continue;}
			if(root[0] == '\\'){str[length++] = '/'; root++; continue;}
			str[length++] = *root++;
		}
	}else{
		str[length++] = '/';
	}

	if(file){
		if(str[length-1] != '/')str[length++] = '/';
		while(*file){
			if(file[0] == '/' && file[1] == '/'){file++; continue;}
			if(file[0] == '\\' && file[1] == '\\'){file++; continue;}
			if(file[0] == '\\'){str[length++] = '/'; file++; continue;}
			str[length++] = *file++;
		}
	}

	return str;
}

char* cutString(char* str, int start, int end)
{
	int wlen = strlen(str);
	int slen = 0;
	char* buf = NULL;

	if(wlen == 0)return initBuffer(NULL, 0);

	while(end < 0)end += wlen;
	if(end == 0 || end > wlen)end = wlen + 1;

	while(start < 0)start += wlen;
	if(start > wlen)start = wlen;

	if((slen = end - start) < 0)return NULL;
	if((buf = initBuffer(NULL, slen)) == NULL)return NULL;

	memcpy(buf, str+start, slen);
	buf[slen] = 0;

	return buf;
}

char* spliseString(char* str, int start, int cutlen, char* buf)
{
	unsigned int buflen = (buf? strlen(buf): 0);
	unsigned int wlen = strlen(str);

	//if(cutlen < 0)start += cutlen, cutlen = 0 - cutlen;
	if(cutlen < 0 || start + cutlen > wlen)cutlen = wlen - start;

	memmove(str + start + buflen, str + start + cutlen, wlen + 1 - (start + cutlen));
	if(buf)memcpy(str + start, buf, buflen);

	return str;
}

int intString(char* str)
{
	int digit = 10;
	int pn = 1;
	int ret = 0;
	char item = 0;
	if((str = skipString(str)) == 0)return 0;

	if(str[0] == '0' && str[1] == 'x' || str[1] == 'X')digit = 16, str += 2;
	else if(str[0] == '0' && str[1] == 'b' || str[1] == 'B')digit = 2, str += 2;
	else if(str[0] == '0')digit = 8, str++;
	else if(str[0] == '-' && '0' < str[1] && str[1] <= '9')pn = -1, str++;
	else if(str[0] == '+' && '0' < str[1] && str[1] <= '9')pn = 1, str++;

	while(1){
		item = *str++;
		if(digit == 16){
			if('a' <= item && item <= 'f')item = item - 39;// 39 = 'a' - '0' - 10;
			else if('A' <= item && item <= 'F')item = item - 7;// 7 = 'A' - '0' - 10;
		}

		if(item < '0' || item >= '0' + digit)break;
		ret *= digit;
		ret += item - '0';
	}

	return pn*ret;
}

double floatString(char *str)
{
	double ret = 0;
	long long ipart = 0;
	long long fpart = 0;
	int ipn = 1;
	int fpn = 1;
	int index = 0;
	if((str = skipString(str)) == 0)return (ret);

	if(str[0] == '-')ipn = -1, str++;
	else if(str[0] == '+')ipn = 1, str++;

	while(*str >= '0' && *str <= '9'){
		ipart *= 10;
		ipart += *str - '0';
		str++;
	}

	if(str[0] == '.')str++;
	//else return ipn * (ret += ipart);
	
	while(*str >= '0' && *str <= '9'){
		fpart *= 10;
		fpart += *str - '0';
		str++;
	}

	while(fpart){
		ret += fpart % 10;
		ret /= 10;
		fpart /= 10;
	}

	if(str[0] == 'e' || str[0] == 'E')str++;
	else return ipn * (ret += ipart);

	if(str[0] == '-')fpn = -1, str++;
	else if(str[0] == '+')fpn = 1, str++;

	while(*str >= '0' && *str <= '9'){
		index *= 10;
		index += *str - '0';
		str++;
	}
	
	index *= fpn;
	ret += ipart;
	while(index > 0){
		ret *= 10;
		index--;
	}
	while(index < 0){
		ret /= 10;
		index++;
	}

	return ipn * ret;

}

