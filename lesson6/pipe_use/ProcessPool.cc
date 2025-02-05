/*
 * @Author: CODE_OF_ZHANG 3363967646@qq.com
 * @Date: 2025-02-05 14:31:43
 * @LastEditors: CODE_OF_ZHANG 3363967646@qq.com
 * @LastEditTime: 2025-02-05 21:50:06
 * @FilePath: /lesson6/pipe_use/ProcessPool.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "Task.hpp"
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <ctime>
const int processnum = 10;
std::vector<task_t> tasks;

// 先描述
class channel
{
public:
    channel(int cmdfd, pid_t slaverid, const std::string &processname)
        : _cmdfd(cmdfd), _slaverid(slaverid), _processname(processname)
    {
    }

public:
    int _cmdfd;               // 发送任务的文件描述符
    pid_t _slaverid;          // 子进程的 PID
    std::string _processname; // 子进程的名字 --> 方便打印日志
};

void slaver()
{
    while (true)
    {
        // std::cout << getpid() << " - " << "read fd is : " << rfd << std::endl;
        // sleep(10000);
        int cmdcode = 0;
        int n = read(0, &cmdcode, sizeof(int));
        if (n == sizeof(int))
        {
            // 执行 cmdcode 对应的任务
            std::cout << "slaver say@ get a command: " << getpid() << " : cmdcode: " << cmdcode << std::endl;
            if (cmdcode >= 0 && cmdcode < tasks.size())
                tasks[cmdcode]();
            if(n == 0)
                break;
        }
    }
}

void InitProcessPool(std::vector<channel> *channels)
{
    // 1. 初始化
    for (int i = 0; i < processnum; i++)
    {
        int pipefd[2]; // 临时空间
        int n = pipe(pipefd);
        assert(!n);
        (void)n;

        pid_t id = fork();
        if (id == 0)
        {
            // child
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            slaver();
            // slaver(pipefd[0]);
            std::cout << "process : " << getpid() << " quit" << std::endl;
            exit(0);
        }
        // father
        close(pipefd[0]);

        // 添加 channel 字段
        std::string name = "process-" + std::to_string(i);
        channels->push_back(channel(pipefd[1], id, name));
    }
}

// DebugTest
void DebugTest(const std::vector<channel> &channels)
{
    // test
    for (const auto &c : channels)
    {
        std::cout << c._cmdfd << " " << c._slaverid << " " << c._processname << std::endl;
    }
}

void Menu()
{
    std::cout << "####################################" << std::endl;
    std::cout << "##### 1. 更新日志  2. 刷新野区  #####" << std::endl;
    std::cout << "##### 3. 软件更新  4. 技能释放  #####" << std::endl;
    std::cout << "#####             0. 退出游戏  #####" << std::endl;
    std::cout << "####################################" << std::endl;
}

void ctrlSlaver(const std::vector<channel> &channels)
{
    int which = 0;
    // int cnt = 5;
    while (true)
    {
        int select = 0;
        Menu();
        std::cout << "Please Enter@ ";
        std::cin >> select;
        if (select <= 0 || select >= 5)
            break;
        // 选择任务
        //  int cmdcode = rand() % 20;
        int cmdcode = select - 1;
        // 选择进程
        // int processpos = rand() % channels.size();
        std::cout << "father say: " << "cmdcode: " << cmdcode << "already sendto " << channels[which]._slaverid
                  << " process name: " << channels[which]._processname << std::endl;
        // 发送任务
        write(channels[which]._cmdfd, &cmdcode, sizeof(cmdcode));
        which++;
        // 防止越界
        which %= channels.size();
        // cnt--;
        sleep(1);
    }
}

void QuitProcess(const std::vector<channel> &channels)
{
    for (const auto &e : channels)
        close(e._cmdfd);
    // sleep(5);
    for (const auto &e : channels)
        waitpid(e._slaverid, nullptr, 0);
}

int main()
{
    LoadTask(&tasks);
    // 随机数种子
    srand(time(nullptr) ^ getpid() ^ 1023);
    // 在组织
    std::vector<channel> channels;
    InitProcessPool(&channels);

    // test
    DebugTest(channels);
    // 2. 开始控制子进程
    ctrlSlaver(channels);

    // 3. 清理收尾
    QuitProcess(channels);
    return 0;
}