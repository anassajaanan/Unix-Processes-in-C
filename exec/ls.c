#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv, char **envp)
{
	int pid = fork();
	if (pid == -1)
	{
		return (1);
	}
	else if (pid == 0)
	{
		int fd = open("ls.txt", O_WRONLY | O_CREAT, 0777);
		if (fd == -1)
		{
			perror("open");
			return (EXIT_FAILURE);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		char *args[] = {"ls", "-la", "../../", NULL};
		execve("/bin/ls", args, envp);
	}
	else
	{
		wait(NULL);
		printf("The ls command has been succefully saved in ls.txt\n");
	}
	return (0);
}