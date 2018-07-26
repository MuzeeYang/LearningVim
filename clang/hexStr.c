#include "string.h"

int _hexChar2Num(char c)
{
	if('0' <= c && c <= '9')
		return (c - '0');
	else if('A' <= c && c <= 'F')
		return (c - 'A' + 10);
	else if('a' <= c && c <= 'f')
		return (c - 'a' + 10);
	else
		return -1;
}

int hexStr2Num(char* str, int* num)
{
	int i;
	int ret = 0;
	int limit = 0;
	int tmp;

	if(!*str)
		return 0;

	while(*str == 10 || *str == 9 || *str == 32)str++;
	limit = (strlen(str) < 8)? strlen(str): 8;
	
	i = 0;
	while((tmp = _hexChar2Num(str[i++])) >= 0)
	{
		ret <<= 4;
		ret |= tmp;
	}
	*num = ret;
	return i;
}

int num2HexStr(int num, char* str)
{
	int i = 8,j,tmp;

	if(!num)
		return 0;

	while(!((num >> ((--i)*4)) & 0xf));

	for(j = 0; i+1 > 0; i--)
	{
		tmp = (num >> (i*4)) & 0xf;

		if(0 <= tmp && tmp < 10)
			tmp += '0';
		else if(10 <= tmp && tmp < 16)
			tmp = tmp - 10 + 'A';
		
		str[j++] = tmp;
	}
	str[j] = 0;
	return j;
}

int strReverse(char* str)
{
	int len = strlen(str);
	int i, j;

	for(i = 0, j = len-1; i < j; i++, j--)
	{
		str[len] = str[i];
		str[i] = str[j];
		str[j] = str[len];
	}
	str[len] = 0;
	return len;
}

