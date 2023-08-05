#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd = open("pipe", O_WRONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	char buufer[] = "I'm Anas Ajaanana and I love 42!!\n";
	if (write(fd, buufer, sizeof(buufer)) == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return 0;
}