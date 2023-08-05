#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	int fd[2];

	if (pipe(fd) == -1)
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
		int num;
		for (int i = 0; i < 5; i++)
		{
			if (read(fd[0], &num, sizeof(int)) == -1)
			{
				perror("read");
				return (EXIT_FAILURE);
			}

			printf("Child Recieving ... %d\n", num);
			num = num * 4;
			if (write(fd[1], &num, sizeof(int)) == -1)
			{
				perror("write");
				return (EXIT_FAILURE);
			}
			printf("Child Sending ... %d\n", num);
		}
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		//parent process
		for (int i = 0; i < 5; i++)
		{
			int num = rand() % 20;
			if (write(fd[1], &num, sizeof(int)) == -1)
			{
				perror("write");
				return (EXIT_FAILURE);
			}
			printf("Parent Sending ... %d\n", num);
		}
		wait(NULL);
		for (int i = 0; i < 5; i++)
		{
			int new_num;
			if (read(fd[0], &new_num, sizeof(int)) == -1)
			{
				perror("read");
				return (EXIT_FAILURE);
			}
			printf("Parent Receieving ... %d\n", new_num);
		}
		close(fd[1]);
		close(fd[0]);
	}
	return (0);
}