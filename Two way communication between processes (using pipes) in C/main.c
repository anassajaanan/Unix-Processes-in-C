#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>


int main(void)
{
	int fd1[2]; // parent will write to it and child will read from it
	int fd2[2]; // parent will read from it and child will write to it

	if (pipe(fd1) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	if (pipe(fd2) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	int pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// child process
		close(fd1[1]);
		close(fd2[0]);
		int num;
		for (int i = 0; i < 5; i++)
		{
			if (read(fd1[0], &num, sizeof(int)) == -1)
			{
				perror("read");
				return (EXIT_FAILURE);
			}

			// printf("Child Recieving ... %d\n", num);
			num = num * 4;
			if (write(fd2[1], &num, sizeof(int)) == -1)
			{
				perror("write");
				return (EXIT_FAILURE);
			}
			// printf("Child Sending ... %d\n", num);
		}
		close(fd1[0]);
		close(fd2[1]);
	}
	else
	{
		//parent process
		close(fd1[0]);
		close(fd2[1]);
		for (int i = 0; i < 5; i++)
		{
			int num = rand() % 20;
			if (write(fd1[1], &num, sizeof(int)) == -1)
			{
				perror("write");
				return (EXIT_FAILURE);
			}
			printf("Parent Sending ... %d\n", num);
		}
		close(fd1[1]);
		for (int i = 0; i < 5; i++)
		{
			int new_num;
			if (read(fd2[0], &new_num, sizeof(int)) == -1)
			{
				perror("read");
				return (EXIT_FAILURE);
			}
			printf("Parent Receieving ... %d\n", new_num);
		}
		close(fd2[0]);
	}
	return (0);
}