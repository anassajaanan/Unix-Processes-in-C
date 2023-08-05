#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	int id = fork();
	if (id == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (id == 0)
	{
		printf("Child process\n");
		int num[2];
		read(pipefd[0], num, sizeof(num));
		close(pipefd[0]);
		// printf("From, child, the result is %d\n", num[0] + num[1]);
		int result = num[0] + num[1];
		write(pipefd[1], &result, sizeof(result));
		close(pipefd[1]);
	}
	else
	{
		printf("Parent process\n");
		int num[2] = {7, 5};
		write(pipefd[1], num, sizeof(num));
		close(pipefd[1]);
		wait(NULL);

		int result;
		read(pipefd[0], &result, sizeof(result));
		printf("From, parent, the result is %d\n", result);
		close(pipefd[0]);


		exit(EXIT_SUCCESS);

	}
}