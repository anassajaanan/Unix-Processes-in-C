#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>


int main(void)
{
	int array[10];
	int fd = open("pipe", O_WRONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 10; i++)
		array[i] = rand() % 100;
	for (int i = 0; i < 10; i++)
	{
		if (write(fd, &array[i], sizeof(int)) == -1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
		printf ("Writing %d\n", array[i]);
	}
	close(fd);
	fd = open("pipe", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	int total;
	if (read(fd, &total, sizeof(total)) == -1)
	{
		perror("read");
		return (EXIT_FAILURE);
	}
	printf("The total of generated random numbers is : %d\n", total);
	close(fd);
	return (0);
}