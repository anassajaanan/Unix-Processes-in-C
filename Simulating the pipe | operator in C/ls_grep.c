#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
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
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		char *args[] = {"ls", "-la", "../../", NULL};
		int err = execve("/bin/ls", args, envp);
		if (err == -1)
		{
			perror("execve");
			return (EXIT_FAILURE);
		}
	}
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
		char *args2[] = {"grep", "42", NULL};
		int err2 = execve("/usr/bin/grep", args2, envp);
		if (err2 == -1)
		{
			perror("execve");
			return (EXIT_FAILURE);
		}
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
	printf("I'm the parent and I control my children\n");
	return (0);
}