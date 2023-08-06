#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fd[3][2];
	for (int i = 0; i < 3; i++)
	{
		if (pipe(fd[i]) == -1)
		{
			perror("pipe");
			return (EXIT_FAILURE);
		}
	}
	int pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid1 == 0)
	{
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		int n;
		if (read(fd[0][0], &n, sizeof(int)) == -1)
		{
			perror("read");
			return (EXIT_FAILURE);
		}
		close(fd[0][0]);
		n += 5;
		if (write(fd[1][1], &n, sizeof(int)) == -1)
		{
			perror("write");
			return (EXIT_FAILURE);
		}
		close(fd[1][1]);
		return (0);
	}
	int pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid2 == 0)
	{
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);
		int n;
		if (read(fd[1][0], &n, sizeof(int)) == -1)
		{
			perror("read");
			return (EXIT_FAILURE);
		}
		close(fd[1][0]);
		n += 5;
		if (write(fd[2][1], &n, sizeof(int)) == -1)
		{
			perror("write");
			return (EXIT_FAILURE);
		}
		close(fd[2][1]);
		return (0);
	}
	close(fd[0][0]);
	close(fd[1][0]);
	close(fd[1][1]);
	close(fd[2][1]);
	int x = 0;
	if (write(fd[0][1], &x, sizeof(int)) == -1)
	{
		perror("write");
		return (EXIT_FAILURE);
	}
	close(fd[0][1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	if (read(fd[2][0], &x, sizeof(int)) == -1)
	{
		perror("read");
		return (EXIT_FAILURE);
	}
	printf("the total is : %d\n", x);
	close(fd[2][0]);
}