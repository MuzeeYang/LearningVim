#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"

int optionFd = 0;

int main(int argc, char** argv)
{
	char buf[256];
	if((optionFd = open("options", O_RDWR|O_CREAT, 0666)) < 0)
		goto openFailed;

	SaveStr("item6", "1234567");

	close(optionFd);
openFailed:
	return 0;
}

int SaveStr(char* name, char* value)
{
	char buf[256];
	char* bufRest = NULL;
	off_t readPos = 0;
	off_t fileSize = lseek(optionFd, 0, SEEK_END);
	off_t restSize = 0;

	lseek(optionFd, 0, SEEK_SET);
	while(readPos != fileSize)
	{
		readPos = readLine(optionFd, buf);
		if(strncmp(buf, name, strlen(name)) == 0 && buf[strlen(name)] == '=')
		{
			if((bufRest = malloc(fileSize - readPos)) == NULL)
				goto restBufferFailed;

			restSize = read(optionFd, bufRest, fileSize-readPos);
			break;
		}
	}
	sprintf(buf, "%s=%s\n", name, value);
	lseek(optionFd, readPos, SEEK_SET);
	write(optionFd, buf, strlen(buf));
	if(bufRest)
	{
		write(optionFd, bufRest, restSize);

		fileSize = lseek(optionFd, 0, SEEK_CUR);
		ftruncate(optionFd, fileSize);
	}

restBufferFailed:
	lseek(optionFd, 0, SEEK_SET);
	return 0;
}

int readLine(int fd, char* buf)
{
	char c;
	size_t curPos = lseek(fd, 0, SEEK_CUR);
	while(read(fd, &c, 1) != 0)
	{
		if(c == '\n')
			break;
		*buf++ = c;
	}
	*buf = 0;

	return curPos;
}
