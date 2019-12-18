#include "stdio.h"

int main(int argc, char** argv)
{
	int fd = -1;
	fd = open("/dev/demo", 0);
	if(fd < 0){
		printf("open failed.\n");
	}

	close(fd);
	return 0;
}

