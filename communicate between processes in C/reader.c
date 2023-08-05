#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int num;
	int sum = 0;
	int fd = open("pipe", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	for (int i = 0; i < 10; i++)
	{
		if (read(fd, &num, sizeof(int)) == -1)
		{
			perror("read");
			return (EXIT_FAILURE);
		}
		printf("Recieving : %d\n", num);
		sum += num;
	}
	close(fd);
	fd = open("pipe", O_WRONLY);
	if (fd == -1)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (write(fd, &sum, sizeof(sum)) == -1)
	{
		perror("write");
		return (EXIT_FAILURE);
	}
	close(fd);
	return (0);
}