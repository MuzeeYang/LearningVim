#include "stdio.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "comSort.h"
#include "string.h"

int main(int argc, char** argv)
{
	char str[256] = "Systime=1,TimeZone=8,TimeZoneMin=0";
	char *equal, *quota;

	equal = strchr(str, '=');
	quota = strchr(str, ',');

	printf("name = %s, name len = %d\n", &str[0], equal - (str+0));
	printf("name = %s, name len = %d\n", equal + 1, quota - (equal+1));
	return 0;
}
