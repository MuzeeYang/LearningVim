#include <stdio.h>
#include "skiString.h"

int main(int argc, char **argv)
{
	//printf("%s alter to: res = %lf\n", argv[1], floatString(argv[1]));
	char buf[128] = {0};
	memcpy(buf, argv[1], strlen(argv[1]));

	printf("%s into %s: %s\n", argv[2], argv[1], spliseString(buf, 0, -1, argv[2]));
	return 0;
}
