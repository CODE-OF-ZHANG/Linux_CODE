#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define N 10

void RunChild()
{
	int cnt = 5;
	while(cnt)
	{
		printf("I am Child Process, pid: %d, ppid: %d\n", getpid(), getppid());
		sleep(1);
		cnt--;
	}
}

int main()
{
	// wait
	for(int i = 0; i < N; i++)
	{
		pid_t id = fork();
		if(id == 0)
		{
			RunChild();
			exit(i);
		}
		printf("create child process: %d success\n", id);
	}
	return 0;
}
