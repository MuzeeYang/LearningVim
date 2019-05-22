#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "skiString.h"
#include "skiJSON.h"


PObject createObject()
{
	PObject pobj = NULL;
	if(pobj = malloc(sizeof(TObject))){
		memset(pobj, 0, sizeof(TObject));
	}
	return pobj;
}

int freeObject(PObject pobj)
{
	if(pobj == NULL)return -1;
	if((pobj->type == OBJECT_TYPE_ARRAY || pobj->type == OBJECT_TYPE_OBJECT) && pobj->value.child)freeObject(pobj->value.child);
	if(pobj->next)freeObject(pobj->next);

	if(pobj->name)free(pobj->name);
	if(pobj->type == OBJECT_TYPE_STRING)free(pobj->value.string);
	free(pobj);
	return 0;
}


static char* wrapJSON_value(PObject pobj, char* buf, unsigned int* length);

static char* wrapJSON_null(PObject pobj, char* buf, unsigned int* length)
{
	char null[] = "null";
	buf = initBuffer(buf, *length + sizeof(null));
	if(buf == NULL)return NULL;
	
	memcpy(buf + *length, null, sizeof(null));
	*length += sizeof(null) - 1;
	return buf;
}

static char* wrapJSON_bool(PObject pobj, char* buf, unsigned int* length)
{
	if(pobj->value.boolen){
		buf = initBuffer(buf, *length + 5);
		if(buf == NULL)return NULL;

		strcpy(buf + *length, "true");
		*length += 5 - 1;
	}else{
		buf = initBuffer(buf, *length + 6);
		if(buf == NULL)return NULL;

		strcpy(buf + *length, "false");
		*length += 6 - 1;
	}
	return buf;
}

static char* wrapJSON_number(PObject pobj, char* buf, unsigned int* length)
{
	buf = initBuffer(buf, *length + 64);
	if(buf == NULL)return NULL;

	*length += sprintf(buf + *length, "%f", pobj->value.number) - 1;
	return buf;
}

static char* wrapJSON_string(PObject pobj, char* buf, unsigned int* length)
{
	buf = initBuffer(buf, *length + strlen(pobj->value.string) + 3);
	if(buf == NULL)return NULL;

	*length += sprintf(buf + *length, "\"%s\"", pobj->value.string);
	return buf;
}

static char* wrapJSON_name(PObject pobj, char* buf, unsigned int* length)
{
	buf = initBuffer(buf, *length + strlen(pobj->name) + 3);
	if(buf == NULL)return NULL;

	*length += sprintf(buf + *length, "\"%s\"", pobj->name);
	return buf;
}

static char* wrapJSON_array(PObject pobj, char* buf, unsigned int* length)
{
	int num = 0;
	PObject child = pobj->value.child;
	while(child)num++, child = child->next;

	if(num){
		buf = initBuffer(buf, *length + 2*(num-1) + 2);
		if(buf == NULL)return NULL;

		buf[(*length)++] = '[';
		child = pobj->value.child;
		while(child){
			wrapJSON_value(child, buf, length);
			if(--num)buf[(*length)++] = ',', buf[(*length)++] = ' ';
			child = child->next;
		}
		buf[(*length)++] = ']';
	}else{
		buf = initBuffer(buf, *length + 2);
		if(buf == NULL)return NULL;

		buf[(*length)++] = '[';
		buf[(*length)++] = ']';
	}
	return buf;
}

static char* wrapJSON_object(PObject pobj, char* buf, unsigned int* length)
{
	int num = 0;
	PObject child = pobj->value.child;
	while(child)num++, child = child->next;

	if(num){
		buf = initBuffer(buf, *length + 2*(num-1) + 2 + num);
		if(buf == NULL)return NULL;

		buf[(*length)++] = '{';
		child = pobj->value.child;
		while(child){
			buf = wrapJSON_name(child, buf, length);
			buf[(*length)++] = ':';
			buf = wrapJSON_value(child, buf, length);
			if(--num)buf[(*length)++] = ',', buf[(*length)++] = ' ';
			child = child->next;
		}
		buf[(*length)++] = '}';
	}else{
		buf = initBuffer(buf, *length + 2);
		if(buf == NULL)return NULL;

		buf[(*length)++] = '{';
		buf[(*length)++] = '}';
	}
	return buf;
}

static char* wrapJSON_value(PObject pobj, char* buf, unsigned int* length)
{
	switch(pobj->type){
		case OBJECT_TYPE_NULL:buf = wrapJSON_null(pobj, buf, length);break;
		case OBJECT_TYPE_BOOL:buf = wrapJSON_bool(pobj, buf, length);break;
		case OBJECT_TYPE_NUMBER:buf = wrapJSON_number(pobj, buf, length);break;
		case OBJECT_TYPE_STRING:buf = wrapJSON_string(pobj, buf, length);break;
		case OBJECT_TYPE_ARRAY:buf = wrapJSON_array(pobj, buf, length);break;
		case OBJECT_TYPE_OBJECT:buf = wrapJSON_object(pobj, buf, length);break;
	}
	return buf;
}

char* wrapJSON(PObject pobj)
{
	char* buffer = NULL;
	unsigned int length = 0;
	//buffer = initBuffer(NULL, 0);
	if(pobj == NULL)return NULL;
	
	buffer = wrapJSON_value(pobj, buffer, &length);

	return buffer;
}

static char* parseJSON_value(PObject pobj, char* buf);

static char* parseJSON_null(PObject pobj, char* buf)
{
	pobj->type = OBJECT_TYPE_NULL;
	return skipString(buf + 4);
}

static char* parseJSON_bool(PObject pobj, char* buf)
{
	pobj->type = OBJECT_TYPE_BOOL;
	if(strncmp(buf, "true", 4) == 0){
		pobj->value.boolen = 1;
		return skipString(buf + 4);
	}else if(strncmp(buf, "false", 5) == 0){
		pobj->value.boolen = 0;
		return skipString(buf + 5);
	}
}

static char* parseJSON_number(PObject pobj, char* buf)
{
	int sign = 1, scale = 0, subsign = 1, subscale = 0;
	double num = 0;

	if(*buf == '+')buf++;
	else if(*buf == '-')buf++, sign = -1;
	while(*buf >= '0' && *buf <= '9')num = num*10 + *buf++ - '0';
	if(*buf == '.' && (buf[1] >= '0' && buf[1] <= '9')){
		buf++;
		do{num = num*10 + *buf++ - '0'; scale--;}while(*buf >= '0' && *buf <= '9');
	}
	if(*buf == 'e' || *buf == 'E'){
		buf++;
		if(*buf == '+')buf++;
		else if(*buf == '-')buf++, subsign = -1;
		while(*buf >= '0' && *buf <= '9')subscale = subscale*10 + *buf++ - '0';
	}

	scale = subsign*subscale + scale;
	if(scale < 0)while(scale++)num /= 10;
	else while(scale--)num *= 10;

	pobj->type = OBJECT_TYPE_NUMBER;
	pobj->value.number = num;
	return skipString(buf);
}

static char* parseJSON_string(PObject pobj, char* buf)
{
	int len = 0;
	if(*buf == '\"')buf++;
	while(buf[len] != '\"')if(buf[len++] == '\\')len++;

	pobj->value.string = malloc(len + 1);
	if(pobj->value.string == NULL)return NULL;

	pobj->type = OBJECT_TYPE_STRING;
	memcpy(pobj->value.string, buf, len);
	pobj->value.string[len] = 0;
	return skipString(buf + len + 1);
}

static char* parseJSON_name(PObject pobj, char* buf)
{
	int len = 0;
	if(*buf == '\"')buf++;
	while(buf[len] != '\"')if(buf[len++] == '\\')len++;

	pobj->name = malloc(len + 1);
	if(pobj->name == NULL)return NULL;

	//pobj->type = OBJECT_TYPE_STRING;
	memcpy(pobj->name, buf, len);
	pobj->name[len] = 0;
	return skipString(buf + len + 1);
}


static char* parseJSON_array(PObject pobj, char* buf)
{
	PObject child = NULL;
	PObject cursor = NULL;
	pobj->type = OBJECT_TYPE_ARRAY;
	if(*buf == '[')buf = skipString(buf + 1);
	if(*buf == ']')return skipString(buf + 1);

	pobj->value.child = child = createObject();
	if(child == NULL)return NULL;

	buf = parseJSON_value(child, buf);

	while(*buf != ']'){
		if(*buf == ',')buf = skipString(buf + 1);
		cursor = child;
		child = createObject();
		if(child == NULL)return NULL;

		buf = parseJSON_value(child, buf);
		cursor->next = child;
	}

	return skipString(buf + 1);
}

static char* parseJSON_object(PObject pobj, char* buf)
{
	PObject child = NULL;
	PObject cursor = NULL;
	pobj->type = OBJECT_TYPE_OBJECT;
	if(*buf == '{')buf = skipString(buf + 1);
	if(*buf == '}')return skipString(buf + 1);

	pobj->value.child = child = createObject();
	if(child == NULL)return NULL;

	buf = parseJSON_name(child, buf);
	if(*buf == ':')buf = skipString(buf + 1);
	buf = parseJSON_value(child, buf);

	while(*buf != '}'){
		if(*buf == ',')buf = skipString(buf + 1);
		cursor = child;
		child = createObject();
		if(child == NULL)return NULL;

		buf = parseJSON_name(child, buf);
		if(*buf == ':')buf = skipString(buf + 1);
		buf = parseJSON_value(child, buf);
		cursor->next = child;
	}

	return skipString(buf + 1);
}

static char* parseJSON_value(PObject pobj, char* buf)
{
	if(strncmp(buf, "null", 4) == 0)buf = parseJSON_null(pobj, buf);
	else if(strncmp(buf, "true", 4) == 0 || strncmp(buf, "false", 5) == 0)buf = parseJSON_bool(pobj, buf);
	else if(*buf == '-' || *buf == '+' || (*buf >= '0' && *buf <= '9'))buf = parseJSON_number(pobj, buf);
	else if(*buf == '\"')buf = parseJSON_string(pobj, buf);
	else if(*buf == '[')buf = parseJSON_array(pobj, buf);
	else if(*buf == '{')buf = parseJSON_object(pobj, buf);

	return skipString(buf);
}

PObject parseJSON(char* buffer)
{
	PObject pobj = createObject();
	if(pobj == NULL)return NULL;

	buffer = parseJSON_value(pobj, skipString(buffer));
	return pobj;
}
