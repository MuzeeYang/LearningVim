#include "stdlib.h"

int main(int argc, char** argv)
{
	int pipeFd[2];
	int pid;
	char buffer[256] = {0};

	pipe(pipeFd);

	pid = vfork();
	if(pid == 0)
	{
		dup2(pipeFd[1], 1);
		system(argv[1]);
		putchar(0);
		exit(ret);
	}
	else
	{
		read(pipeFd[0], buffer, sizeof(buffer))
		waitpid(pid, NULL, 0);
	}
	
	return 0;
}
