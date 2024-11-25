#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("begin: 这是一个进程, pid: %d, ppid: %d\n", getpid(), getppid());
    pid_t ret = fork();
    if (ret == 0)
    {
        while (1)
        {
            printf("这是一个子进程, pid: %d, ppid: %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else if (ret > 0)
    {
        while (1)
        {
            printf("这是一个父进程, pid: %d, ppid: %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else
    {
        // err
    }

    // while(1)
    //{
    //     printf("begin: 这是一个进程, pid: %d, ppid: %d\n", getpid(), getppid());
    //     sleep(1);
    // }
    return 0;
}

