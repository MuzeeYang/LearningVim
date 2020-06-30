#include "malloc.h"
#include "string.h"
#include "skiDict.h"


static int _freeValueByNode(PMapNode pNode)
{
	PDictObj object = NULL;
	if(pNode){
		object = GET_DICT_BY_KEY(pNode);
		if(object->key_node.key)free(object->key_node.key);

		switch(object->type){
			case TYPE_STRING:
			case TYPE_STRUCT:
			case TYPE_ARRAY:
				if(object->data.s.ptr)free(object->data.s.ptr);
				object->data.s.ptr = NULL;
				break;
			case TYPE_DICT:
				if(object->data.s.ptr)freeDict(object->data.s.ptr);
				object->data.s.ptr = NULL;
				break;
			default:
				break;
		}
		free(object);
	}
	return 0;
}

void* createDict(unsigned int size)
{
	return createHashMap(size);
}

void freeValueDict(PDictObj object)
{
	if(object)_freeValueByNode(&object->key_node);
}

void freeDict(void* handle)
{
	freeHashMap(handle, _freeValueByNode);
}

PDictObj pushValueDict(DICT_OBJ_TYPE type, void* value, unsigned int vlen)
{
	PDictObj object = NULL;//malloc(sizeof(TDictObj));

	if((object = malloc(sizeof(TDictObj))) == NULL)goto dict_mem_failed;
	
	memset(object, 0, sizeof(TDictObj));
	//object->key_node.key = strdup(key);
	switch(type){
		case TYPE_NUMBER:
		case TYPE_DOUBLE:
		case TYPE_BOOLEN:
			if(vlen <= sizeof(object->data))memcpy(&object->data.l, value, vlen);
			break;
		case TYPE_STRING:
		case TYPE_STRUCT:
			if((object->data.s.ptr = malloc(vlen)) == NULL)goto dict_vmem_failed;
			memcpy(object->data.s.ptr, value, vlen);
			object->data.s.len = vlen;
			break;
		case TYPE_ARRAY:
			if((object->data.s.ptr = malloc(vlen * sizeof(TDictObj))) == NULL)
				goto dict_vmem_failed;
			memset(object->data.s.ptr, 0, vlen * sizeof(TDictObj));
			object->data.s.len = vlen;
			break;
		case TYPE_DICT:
			if((object->data.s.ptr = createDict(vlen)) == NULL)goto dict_vmem_failed;
			object->data.s.len = vlen;
			break;
		case TYPE_NULL:
		default:
			break;
	}
	object->type = type;

	return object;

dict_vmem_failed:
	free(object);
dict_mem_failed:
	return NULL;
}

int pushDict(void* handle, char* key, PDictObj object)
{
	if(object == NULL || handle == NULL)goto dict_arg_failed;

	object->key_node.key = strdup(key);
	if(object->key_node.key == NULL)goto dict_key_failed;

	_freeValueByNode(insertHashMap(handle, &object->key_node));
	return 0;

	free(object->key_node.key);
dict_key_failed:
dict_arg_failed:
	return -1;
}

PDictObj popDict(void* handle, char* key)
{
	PMapNode pNode = NULL;//searchHashMap(handle, key);

	if(handle == NULL)return 0;
	pNode = eraseHashMap(handle, key);
	if(pNode == NULL)return 0;

	return GET_DICT_BY_KEY(pNode);
}

#ifdef DICT_JSON_ENABLE
static char* _wrapArray(PDictObj pArray, unsigned int size, char* buf, unsigned int* len);
static char* _wrapDict(void* handle, char* buf, unsigned int* len);

static char* _wrapBool(unsigned char b, char* buf, unsigned int* len)
{
	unsigned int valLen = 0;
	char* val = NULL;
	if(b){
		val = "True";
		valLen = 4;
	}else{
		val = "False";
		valLen = 5;
	}

	if((buf = initBuffer(buf, *len + valLen)) == NULL)return buf;
	memcpy(buf + *len, val, valLen);
	*len += valLen;

	return buf;
}

static char* _wrapString(char* val, char* buf, unsigned int* len)
{
	unsigned int valLen = 0;//strlen(val);
	if(val == NULL)return buf;
	valLen = strlen(val);

	if((buf = initBuffer(buf, *len + valLen + 2)) == NULL)return buf;
	buf[(*len)++] = '"';
	memcpy(buf + *len, val, valLen);
	*len += valLen;
	buf[(*len)++] = '"';

	return buf;
}

static char* _wrapNull(char* buf, unsigned int* len)
{
	char* val = "null";
	unsigned int valLen = 4;

	if((buf = initBuffer(buf, *len + valLen)) == NULL)return buf;
	memcpy(buf + *len, val, valLen);
	*len += valLen;

	return buf;
}

static char* _wrapNumber(long long val, char* buf, unsigned int* len)
{
	char tmp[16] = {0};
	unsigned int valLen = snprintf(tmp, sizeof(tmp), "%lld", val);

	if((buf = initBuffer(buf, *len + valLen)) == NULL)return buf;
	memcpy(buf + *len, tmp, valLen);
	*len += valLen;

	return buf;
}

static char* _wrapDouble(double val, char* buf, unsigned int* len)
{
	char tmp[16] = {0};
	unsigned int valLen = snprintf(tmp, sizeof(tmp), "%lf", val);

	if((buf = initBuffer(buf, *len + valLen)) == NULL)return buf;
	memcpy(buf + *len, tmp, valLen);
	*len += valLen;

	return buf;
}

static char* _wrapObj(PDictObj object, char* buf, unsigned int* len)
{
	if(object == NULL)goto dict_obj_failed;

	switch(object->type){
		case TYPE_BOOLEN:
			buf = _wrapBool(popValueDict_bool(object), buf, len);
			break;
		case TYPE_STRING:
			buf = _wrapString(popValueDict_string(object), buf, len);
			break;
		case TYPE_NULL:
			buf = _wrapNull(buf, len);
			break;
		case TYPE_ARRAY:
			buf = _wrapArray(popValueDict_array(object), popValueDict_length(object), buf, len);
			break;
		case TYPE_DICT:
			buf = _wrapDict(popValueDict_dict(object), buf, len);
			break;
		case TYPE_NUMBER:
			buf = _wrapNumber(popValueDict_long(object), buf, len);
			break;
		case TYPE_DOUBLE:
			buf = _wrapDouble(popValueDict_double(object), buf, len);
			break;
		default:
			break;
	}

dict_obj_failed:
	return buf;
}

static char* _wrapNode(PMapNode pNode, char* buf, unsigned int* len)
{
	unsigned int keyLen = 0, valLen = 0;
	PDictObj object = NULL;
	if(pNode == NULL)goto dict_node_failed;

	keyLen = strlen(pNode->key);
	if((buf = initBuffer(buf, *len + keyLen + 4)) == NULL)goto dict_buf_failed;

	buf[(*len)++] = '"';
	memcpy(buf + *len, pNode->key, keyLen);
	*len += keyLen;
	buf[(*len)++] = '"';
	buf[(*len)++] = ':';
	buf[(*len)++] = ' ';

	object = GET_DICT_BY_KEY(pNode);
	buf = _wrapObj(object, buf, len);

dict_buf_failed:
dict_node_failed:
	return buf;
}

static char* _wrapArray(PDictObj pArray, unsigned int size, char* buf, unsigned int* len)
{
	unsigned int index;
	PDictObj object = NULL;

	if(pArray == NULL)goto dict_array_failed;
	
	if((buf = initBuffer(buf, *len + 2 + (size - 1)*2)) == NULL)goto dict_buf_failed;
	buf[(*len)++] = '[';
	//buf[(*len)++] = ' ';
	for(index = 0; index < size; index++){
		object = pArray + index;
		buf = _wrapObj(object, buf, len);
		if(index + 1 != size){buf[(*len)++] = ','; buf[(*len)++] = ' ';}
	}
	//buf[(*len)++] = ' ';
	buf[(*len)++] = ']';


dict_buf_failed:
dict_array_failed:
	return buf;
}

static char* _wrapDict(void* handle, char* buf, unsigned int* len)
{
	unsigned int keyIdx = 0, keyCnt = 0;
	char** keys;//[10];// = NULL;
	if(handle == NULL)goto dict_handle_failed;

	keyCnt = keysHashMap(handle, NULL);
	if((keys = malloc(keyCnt * sizeof(char*))) == NULL)goto dict_kmem_failed;
	keysHashMap(handle, keys);

	if((buf = initBuffer(buf, *len + 2 + (keyCnt-1)*2)) == NULL)
		goto dict_buf_failed;

	buf[(*len)++] = '{';
	//buf[(*len)++] = ' ';
	for(keyIdx = 0; keyIdx < keyCnt; keyIdx++){
		buf = _wrapNode(searchHashMap(handle, keys[keyIdx]), buf, len);
		if(keyIdx + 1 != keyCnt){buf[(*len)++] = ','; buf[(*len)++] = ' ';}
	}
	//buf[(*len)++] = ' ';
	buf[(*len)++] = '}';

dict_buf_failed:
	free(keys);
dict_kmem_failed:
dict_handle_failed:
	return buf;
}

char* wrapJsonDict(void* handle)
{
	unsigned int len = 0;
	char* buf = NULL;

	if((buf = initBuffer(NULL, 1)) == NULL)return "";
	buf = _wrapDict(handle, buf, &len);
	buf[len] = 0;

	return buf;
}


static void* _parseDict(char* buf);
static PDictObj _parseArray(char* buf);

static PDictObj _parseString(char* buf) 
{
	return pushValueDict(TYPE_STRING, buf, strlen(buf));
}

static PDictObj _parseBool(char* buf)
{
	PDictObj object = NULL;
	char bval = -1;
	if(!strcmp(buf, "true"))bval = 1;
	else if(!strcmp(buf, "false"))bval = 0;

	if(bval >= 0)object = pushValueDict(TYPE_BOOLEN, &bval, sizeof(bval));
	return object;
}

static PDictObj _parseNull(char* buf)
{
	if(!strcmp(buf, "null"))return pushValueDict(TYPE_NULL, NULL, 0);
	else return NULL;
}

static PDictObj _parseNumber(char* buf)
{
	long long int num = 0;
	if(regString(buf, "^[-+]\\?[0-9]*$")){
		num = intString(buf);
		return pushValueDict(TYPE_NUMBER, &num, sizeof(num));
	}

	return NULL;
}

static PDictObj _parseDouble(char* buf)
{
	double f = 0.0;
	f = floatString(buf);

	return pushValueDict(TYPE_DOUBLE, &f, sizeof(f));
}

static PDictObj _parseObj(char* value)
{
	PDictObj ret = NULL;
	unsigned int len = 0;
	if(value == NULL)goto dict_arg_failed;
	if((len = strlen(value)) == 0)goto dict_vallen_failed;

	if(value[0] == '{' && value[len-1] == '}'){
		value[len-1] = 0;
		ret = pushValueDict(TYPE_DICT, _parseDict(value+1), 0);
	}else if(value[0] == '[' && value[len-1] == ']'){
		value[len-1] = 0;
		ret = _parseArray(value+1);
	}else if(value[0] == '"' && value[len-1] == '"'){
		value[len-1] = 0;
		ret = _parseString(value+1);
	}else if(!lowerString(value));
	else if(ret = _parseBool(value));
	else if(ret = _parseNull(value));
	else if(ret = _parseNumber(value));
	else if(ret = _parseDouble(value));
	
dict_lowval_failed:
dict_vallen_failed:
dict_arg_failed:
	return ret;
}

static void _parseNode(void* handle, char* key, char* value)
{
	PDictObj object = NULL;
	unsigned int keyLen = 0;
	if(handle == NULL)goto dict_arg_failed;

	if((object = _parseObj(value)) == NULL)goto dict_obj_failed;

	if((keyLen = strlen(key)) == 0)goto dict_key_failed;
	if(key[0] == '\"' && key[keyLen-1] == '\"')key[keyLen-1] = 0, key++;
	pushDict(handle, key, object);

dict_key_failed:
dict_obj_failed:
dict_arg_failed:
	return;
}

static PDictObj _parseArray(char* buf)
{
	PDictObj object = NULL;
	PDictObj arrobj = NULL;
	PStrWrapper swarr = NULL;
	int i;
	if(buf == NULL)goto dict_arg_failed;

	if((swarr = wrapString(buf, ",")) == NULL)goto dict_swarr_failed;
	if((object = pushValueDict(TYPE_ARRAY, 0, swarr->num)) == NULL)goto dict_obj_failed;
	for(i = 0; i < swarr->num; i++){
		arrobj = _parseObj(washString(swarr->field[i]));
		if(arrobj){
			memcpy(popValueDict_array(object) + i, arrobj, sizeof(TDictObj));
			freeValueDict(arrobj);
		}
	}

dict_obj_failed:
	freeWrapper(swarr);
dict_swarr_failed:
dict_arg_failed:
	return object;
}

static void* _parseDict(char* buf)
{
	PStrWrapper swdict = NULL;
	PStrWrapper swobj = NULL;
	void* handle = NULL;
	int i;
	if(buf == NULL)goto dict_args_failed;
	if((handle = createDict(0)) == NULL)goto dict_handle_failed;

	if((swdict = wrapString(buf, ",")) == NULL)goto dict_swdict_failed;
	//if((swdict = wrapRegex(buf, "\"\\.\\*\":\\.\\*,")) == NULL)goto dict_swdict_failed;
	for(i = 0; i < swdict->num; i++){
		if((swobj = wrapString(swdict->field[i], ":")) == NULL)goto dict_swobj_failed;
		if(swobj->num == 2){
			_parseNode(handle, washString(swobj->field[0]), washString(swobj->field[1]));
		}
		freeWrapper(swobj);
	}

dict_swobj_failed:
	freeWrapper(swobj);
dict_swdict_failed:
dict_handle_failed:
dict_args_failed:
	return handle;
}

void* parseJsonDict(char* buf)
{
	unsigned int bufLen = 0;
	void* handle = NULL;
	if(buf == NULL)goto dict_arg_failed;
	buf = washString(buf);
	bufLen = strlen(buf);
	if(buf[0] != '{' || buf[bufLen - 1] != '}')goto dict_type_failed;

	//if((handle = createDict(0)) == NULL)goto dict_handle_failed;
	
	buf[bufLen-1] = 0;
	handle = _parseDict(buf+1);
	return handle;

	freeDict(handle);
	handle = NULL;
//dict_handle_failed:
dict_type_failed:
dict_arg_failed:
	return handle;
}

#endif




