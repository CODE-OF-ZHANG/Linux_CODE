#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

#define N 2
#define NUM 1024
using namespace std;

// child
void Writer(int wfd)
{
    string s = "hello, I am child";
    pid_t self = getpid();
    int number = 0;

    char buffer[NUM];
    while (true)
    {
        sleep(1);
        // 构建发送字符串
        buffer[0] = 0; // 清空字符串
        // snprintf(buffer, sizeof(buffer), "%s-%d-%d", s.c_str(), self, number++);
        // cout << buffer << endl;
        // 发送/写入字符串给父进程   system call
        // write(wfd, buffer, strlen(buffer)); // strlen(buffer) + 1 ???
        char c = 'c';
        write(wfd, &c, 1);
        number++;
        cout << number << endl;
        if(number >= 5)
            break;
        // sleep(1);
    }
}

// father
void Reader(int rfd)
{
    char buffer[NUM];
    while (true)
    {
        // sleep(50);
        buffer[0] = 0;
        // system call
        ssize_t n = read(rfd, buffer, sizeof(buffer)); // sizeof != strlen
        if (n > 0)
        {
            buffer[n] = 0; // 0 == '\0'
            cout << "father get a message[" << getpid() << "]# " << buffer << endl;
        }
        // File end
        else if (n == 0)
        {
            printf("father read file done...\n");
            break;
        }
        else
            break;
        std::cout << "n: " << n << endl;
    }
}

int main()
{
    int pipefd[N] = {0};
    int n = pipe(pipefd);
    if (n < 0)
        return 1;
    // cout << "pipefd[0]: " << pipefd[0] << ", pipefd[1]: " << pipefd[1] << endl;
    pid_t id = fork();
    if (id < 0)
        return 2;
    // child --> r    father --> w
    if (id == 0)
    {
        // child
        close(pipefd[0]);
        // IPC code
        Writer(pipefd[1]);

        close(pipefd[1]);
        // 关闭子进程
        exit(0);
    }
    // father
    close(pipefd[1]);
    // IPC code
    Reader(pipefd[0]);

    // 回收子进程
    pid_t rid = waitpid(id, nullptr, 0);
    if (rid < 0)
        return 3;
    close(pipefd[0]);
    // child done
    sleep(3);
    return 0;
}
