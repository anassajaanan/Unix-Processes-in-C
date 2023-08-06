#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	int pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid1 == 0)
	{
		close(fd[0]);
		char *args1[] = {"ping", "-c", "5", "google.com", NULL};
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		int err = execve("/sbin/ping", args1, envp);
		if (err == -1)
		{
			printf("An error happen while trying to execute the ping command\n");
			return (3);
		}
		printf("I'm the child\n");
	}
	else
	{
		int pid2 = fork();
		if (pid2 == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		else if (pid2 == 0)
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			char *args2[] = {"grep", "ttl", NULL};
			int err = execve("/usr/bin/grep", args2, envp);
			if (err == -1)
			{
				printf("An error happen while trying to execute the grep command\n");
				return (4);
			}
			printf("I'm the second child\n");
			wait(NULL);
		}
		else
		{
			close(fd[0]);
			close(fd[1]);
			wait(NULL); // Wait for the first child to finish
			wait(NULL); // Wait for the second child to finish
			printf("I'm the parent and I control my children\n");
		}
	}
	return (0);
}
