#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int err;

int main(int argc, char *argv[] , char *envp[])
{
	printf("I'm the main process and this my PID : %d\n", getpid());
	int pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *args[] = {"ls", "-la", NULL};
		// err = 0;
		err = execve("/bin/ls", args, envp);
		if (err == -1)
		{
			printf("Couldn't find the programm!!!\n");
		}
	}
	else
	{
		wait(NULL);
		printf("You see the difference now, I'm the main programm and I have finished\n");
		printf("The exit status is %d\n", err);
	}
	
	return (0);


}