#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// 文件路径, 默认在当前路径下
#define filename "log.txt"

// 利用系统调用接口完成输入重定向
int main()
{
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    // 如果文件打开失败
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    // 输入重定向
    dup2(fd, 1);
    // 利用C语言函数实现
    printf("fd: %d\n", fd);
    printf("hello printf\n");
    fprintf(stdout, "hello fprintf\n");

    // 利用系统调用接口实现
    // char buff[1024];
    // ssize_t s = read(0, buff, sizeof(buff) - 1);
    // if (s > 0)
    // {
    //     buff[s] = '\0';
    //     printf("echo# %s\n", buff);
    // }
    // 关闭文件
    close(fd);
    return 0;
}

// 利用系统调用接口完成输出重定向
// int main()
// {
//     // int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
//     // 追加重定向
//     int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
//     // 如果文件打开失败
//     if (fd < 0)
//     {
//         perror("open");
//         return 1;
//     }
//     dup2(fd, 1);
//     close(fd);
//     const char *msg = "hello Linux\n";
//     // 文件打开成功, 写入内容
//     int cnt = 5;
//     while (cnt)
//     {
//         // 向0号文件描述符中写入
//         // write(0, msg, strlen(msg));

//         // 向1号文件描述符中写入
//         write(1, msg, strlen(msg));
//         cnt--;
//     }

//     // 关闭文件
//     close(fd);
//     return 0;
// }

// 模拟文件重定向原理
// int main()
// {
//     // 关闭1号文件描述符
//     close(1);

//     int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
//     // 如果文件打开失败
//     if (fd < 0)
//     {
//         perror("open");
//         return 1;
//     }
//     // 关闭1号文件描述符后, fd就变成了1
//     // printf("%d\n", fd);
//     const char *msg = "hello Linux\n";
//     // 文件打开成功, 写入内容
//     int cnt = 5;
//     while (cnt)
//     {
//         // 向0号文件描述符中写入
//         // write(0, msg, strlen(msg));

//         // 向1号文件描述符中写入
//         write(1, msg, strlen(msg));
//         cnt--;
//     }

//     // 关闭文件
//     close(fd);
//     return 0;
// }

// 文件描述符的分配原理
// int main()
// {
//     // 关闭2号文件描述符
//     close(2);

//     int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
//     // 如果文件打开失败
//     if (fd < 0)
//     {
//         perror("open");
//         return 1;
//     }
//     // 关闭2号文件描述符后, fd变成了2
//     // 结论: 文件描述符的分配规则是从0开始, 从小到大进行分配的
//     printf("%d\n", fd);
//     const char *msg = "hello Linux\n";
//     // 文件打开成功, 写入内容
//     int cnt = 5;
//     while (cnt)
//     {
//         write(fd, msg, strlen(msg));
//         cnt--;
//     }

//     // 关闭文件
//     close(fd);
//     return 0;
// }
