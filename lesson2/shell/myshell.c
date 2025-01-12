#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT "["
#define RIGHT "]"
#define LABLE "$"
#define DELIM " \t"
#define LINE_SIZE 1024
#define ARGC_SIZE 32
#define EXIT_CODE 44

// 全局变量
int lastcode = 0;
int quit = 0;
extern char **environ;
char commandline[LINE_SIZE];
char *argv[ARGC_SIZE];
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];

// 获取用户名
const char* getusername()
{
	return getenv("USER");
}

// 获取工作路径
void getpwd()
{
	getcwd(pwd, sizeof(pwd));
}
// 交互
void interact(char *cline, int size)
{
	getpwd();
	char hostname[20];
	int  ret = gethostname(hostname, sizeof(hostname));
	if(ret == 0)
	{
		printf(LEFT"%s@%s %s"RIGHT""LABLE" ", getusername(), hostname, pwd);
		char *s = fgets(cline, size, stdin);
		assert(s != NULL);
		(void)s;

		cline[strlen(cline) - 1] = '\0';
	}
	else
	{
		printf("Error getting hostname.\n");
		exit( EXIT_FAILURE);
	}
}

// 字符串切割
int splitstring(char cline[], char *_argv[])
{
	int i = 0;
	_argv[i++] = strtok(cline, DELIM);
	while(_argv[i++] = strtok(NULL, DELIM));  // 故意写的= 
	return i - 1;
}

void normalexcute(char *_argv[])
{

		// 5. 普通命令的执行
		pid_t id = fork();
		// 子进程创建失败
		if(id < 0)
		{
			perror("fork");
			return;
		}
		// 创建成功
		else if(id == 0)
		{
			// 让子进程执行命令
			execvpe(_argv[0], _argv, environ);
			exit(EXIT_CODE);
		}
		else
		{
			// 父进程, 等待子进程执行完命令
			int status = 0;
			pid_t rid = waitpid(id, &status, 0);
			if(rid == id) 
			{
				lastcode = WEXITSTATUS(status);
			}
		}
}

int buildCommand(char *_argv[], int _argc)
{
	if(_argc == 2 && strcmp(_argv[0], "cd") == 0)
	{
		chdir(_argv[1]);
		getpwd();
		sprintf(getenv("PWD"), "%s", pwd);
		return 1;
	}
	// 导入环境变量
	else if(_argc == 2 && strcmp(_argv[0], "export") == 0)
	{
		strcpy(myenv, _argv[1]);
		putenv(myenv);
		return 1;
	}
	// 特殊处理echo命令
	else if(_argc == 2 && strcmp(_argv[0], "echo") == 0)
	{
		// 打印错误码
		if(strcmp(_argv[1], "$?") == 0)
		{
			printf("%d\n", lastcode);
			lastcode = 0;
		}
		// 如果第一个字符是$, 说明是打印一个环境变量
		else if(*_argv[0] == '$')
		{
			char *val = getenv(_argv[1] + 1);
			if(val)  printf("%s\n", val);
		}
		// 如果不是$, 说明就是一个普通的字符, 即输入什么就打印什么
		else
		{
			printf("%s\n", _argv[1]);
		}
		return 1;
	}

	// 给ls命令加上--color选项
	if(strcmp(_argv[0], "ls") == 0)
	{
		_argv[_argc++] = "--color";
		_argv[_argc] = NULL;
	}
	return 0;
}

int main()
{
	while(!quit)
	{
		// 2. 交互问题, 获取命令行
		interact(commandline, sizeof(commandline));
		// commandline -> "ls -a -l\0" -> "ls" "-a" "-l"
		
		// 3. 子串分割问题, 解析命令行
		int argc = splitstring(commandline, argv);

		if(argc == 0)  continue;
		// 4. 指令的判断
		// debug
		// for(int i = 0; argv[i]; i++)  printf("[%d]: %s\n", i, argv[i]);
		// 内建命令的执行
		int n = buildCommand(argv, argc);

		// 普通命令的执行
		if(!n)  normalexcute(argv);
	}
	return 0;
}












