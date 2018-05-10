#include "string.h"

int hexStr2Num(char* str)
{
	int i;
	int ret = 0;
	int limit = (strlen(str) < 8)? strlen(str): 8;
	
	for(i = 0; i < limit; i++)
	{
		ret *= 0x10;

		if('0' <= str[i] && str[i] <= '9')
			ret += (str[i] - '0');
		else if('A' <= str[i] && str[i] <= 'F')
			ret += (str[i] - 'A' + 10);
		else if('a' <= str[i] && str[i] <= 'f')
			ret += (str[i] - 'a' + 10);
	}
	return ret;
}

int num2HexStr(int num, char* str)
{
	int i = 8,j,tmp;

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

