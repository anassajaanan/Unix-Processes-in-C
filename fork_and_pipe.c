#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


// using 4 processes to calculate the sum of an array; that means, each process calculates the sum of 2 elements
// and then the parent process calculates the sum of the 4 sums
// wich make the code super faster.

int main(void)
{
	int A[] = {1, 2 , 5, 4, 8, 55, 99, 23};
	int fd[2];
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	int id = fork();
	if (id == -1)
		return 1;
	if (id == 0)
	{
		int total1;
		int fd2[2];
		if (pipe(fd2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		int id2 = fork();
		if (id2 == -1)
			return 1;
		if (id2 == 0)
		{
			close(fd2[0]);
			int sum1 = 0;
			for (int i = 0; i < 2; i++)
				sum1 += A[i];
			if (write(fd2[1], &sum1, sizeof(sum1)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(fd2[1]);
		}
		else
		{
			close(fd2[1]);
			int sum2 = 0;
			int sum1;
			for (int i = 2; i < 4; i++)
				sum2 += A[i];
			if (read(fd2[0], &sum1, sizeof(sum1)) == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			close(fd2[0]);
			total1 = sum1 + sum2;
			wait(NULL);
			close(fd[0]);
			int sum3 = 0;
			for (int i = 4; i < 6; i++)
				sum3 += A[i];
			sum3 += total1;
			if (write(fd[1], &sum3, sizeof(sum3)) == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(fd[1]);
			wait(NULL);
		}
		
	}
	else
	{
		close(fd[1]);
		int sum4 = 0;
		int sum3;
		for (int i = 6; i < 8; i++)
			sum4 += A[i];
		if (read(fd[0], &sum3, sizeof(sum3)) == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		int total = sum3 + sum4;
		printf("the total is %d\n", total);
	}
	return 0;
}