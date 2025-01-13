#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// 创建文件
int main()
{
	// 打印三个默认的文件流的文件标识符
	printf("stdin->fd: %d\n", stdin->_fileno);
	printf("stdout->fd: %d\n", stdout->_fileno);
	printf("stderr->fd: %d\n", stderr->_fileno);

	// 设置权限
	// umask(0);
	// file descriptor : 文件描述符, fd --> int

	// fopen("log.txt", "w") 的底层封装  --> open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666)
	// int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

	// fopen("log.txt", "a") 的底层封装  --> open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666)
	// int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
	// if (fd < 0)
	// {
	// 	printf("open file error\n");
	// 	return 1;
	// }
	// 写入文件
	// const char *message = "hello file system call\n";  // 第一次写入
	// const char *message = "abc"; // 第二次写入
	// write(fd, message, strlen(message));
	// printf("%d\n", fd);  // 打印结果为3, 因为操作系统默认会打开3个文件流
	//  关闭文件
	// close(fd);
	return 0;
}

// 创建并写入文件
// int main()
// {
// 	// // 改变工作路径
// 	// chdir("/home/zhangxu");
// 	// 打印进程的pid
// 	printf("%d\n", getpid());
// 	// 打开文件的路径和文件名, 默认在当前路径下新建一个文件
// 	FILE *fp = fopen("log.txt", "w");
// 	if (fp == NULL)
// 	{
// 		perror("fopen");
// 		return 1;
// 	}
// 	// 写入文件
// 	const char *message = "hello Linux message";
// 	// 用fwrite写入文件
// 	// fwrite(message, strlen(message), 1, fp);

// 	// 用fprintf写入文件
// 	// fprintf(fp, "%s: %d", message, getpid());

// 	// 用fprintf写入到标准输出流
// 	fprintf(stdout, "%s : %d\n", message, getpid());
// 	// 关闭文件
// 	fclose(fp);
// 	// // 休眠
// 	// sleep(100);
// 	return 0;
// }
