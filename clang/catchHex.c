#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "comSort.h"

int main(int argc, char** argv)
{
	//long len = sysconf(_SC_PAGESIZE);
	int ret = 0;
	if(argc != 3)
	{
		printf("usage:\narg1: input file name\narg2: output file name\n");
		ret = -1;
		goto endProc;
	}

	int fd = open(argv[1], O_RDWR, 0);
	if(fd < 0)
	{
		ret = -2;
		goto endProc;
	}

	long len = lseek(fd, 0, SEEK_END);
	ftruncate(fd, ++len);

	char* pInputFile = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(pInputFile < 0)
	{
		ret = -2;
		goto endProc;
	}
	close(fd);

	short* arrTmp;
	int sizeArr;
	arrTmp = malloc(sizeof(*arrTmp)*(len>>2));
	if(arrTmp == NULL)
	{
		ret = -3;
		goto memFaild;
	}
	sizeArr = getHex2Arr(pInputFile, len, arrTmp);

	quickSort(arrTmp, sizeArr);

	int fdOut = open(argv[2], O_CREAT|O_RDWR|O_TRUNC, 0644);
	if(fdOut < 0)
	{
		ret = -1;
		goto wrFileFailed;
	}

	char buf[10];
	sprintf(buf, "0x%x\n", arrTmp[0]);
	write(fdOut, buf, strlen(buf));

	int i;
	for(i = 1; i < sizeArr; i++)
	{
		if(arrTmp[i] != arrTmp[i-1])
		{
			sprintf(buf, "0x%x\n", arrTmp[i]);
			write(fdOut, buf, strlen(buf));
		}
	}

wrFileFailed:
	free(arrTmp);
memFaild:
	munmap(pInputFile, len);
endProc:
	return ret;
}

int getHex2Arr(char* pInputFile, long len, short* arrTmp)
{
	int sizeArr = 0;
	long curFile = 0;
	int tmp = 0;
	int numLen;

	for(curFile = 0; curFile < len; curFile++)
	{
		if(*(pInputFile+curFile) == 10)
			*(pInputFile+curFile) = 0;
		if(isHex(pInputFile + curFile))
		{
			memset(pInputFile+curFile, 0, 2);
		}
	}

	for(curFile = 0; curFile < len; curFile++)
	{
		if(numLen = hexStr2Num(pInputFile+curFile, &tmp))
		{
			if(tmp != 0)
				arrTmp[sizeArr++] = tmp;
			curFile += numLen;
		}
	}
	return sizeArr;
}

int isHex(char* input)
{
	if(*input == 0)
		return 0;
	else if(*input == '0' && (input[1] == 'x' || input[1] == 'X'))
		return 1;
	else
		return 0;
}
