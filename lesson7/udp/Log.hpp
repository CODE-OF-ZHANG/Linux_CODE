#pragma once

#include <iostream>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 1024

// 日志等级
#define Info 0
#define Debug 1
#define Warn 2
#define Error 3
#define Fatal 4

// 日志落地方式
#define Screen 1    // 屏幕上打印
#define Onefile 2   // 一个文件
#define Classfile 3 // 按照等级分类

// 文件名
#define LogFile "log.txt"

// 日志类
class Log
{
public:
    Log(/* args */)
    {
        printMethod = Screen;
        path = "./log";
    }

    void Enable(int method)
    {
        printMethod = method;
    }

    std::string LevelToString(int level)
    {
        switch (level)
        {
        case Info:
            return "Info";
        case Debug:
            return "Debug";
        case Warn:
            return "Warn";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "None";
        }
    }

    // void logmessage(int level, const char *format, ...)
    // {
    //     time_t t = time(nullptr);
    //     struct tm *ctime = localtime(&t);
    //     char leftbuffer[SIZE];
    //     snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
    //              ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
    //              ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

    //     // va_list s;
    //     // va_start(s, format);
    //     char rightbuffer[SIZE];
    //     vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
    //     // va_end(s);

    //     // 格式：默认部分+自定义部分
    //     char logtxt[SIZE * 2];
    //     snprintf(logtxt, sizeof(logtxt), "%s %s\n", leftbuffer, rightbuffer);

    //     // printf("%s", logtxt); // 暂时打印
    //     printLog(level, logtxt);
    // }

    void printLog(int level, const std::string &Logtxt)
    {
        switch (printMethod)
        {
        case Screen:
            /* code */
            std::cout << Logtxt << std::endl;
            break;
        case Onefile:
            printOneFile(LogFile, Logtxt);
            break;
        case Classfile:
            printClassFile(level, Logtxt);
            break;
        default:
            break;
        }
    }

    void printOneFile(const std::string &logname, const std::string &logtxt)
    {
        std::string _logname = path + logname;
        int fd = open(_logname.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd < 0)
            return;
        write(fd, logtxt.c_str(), logtxt.size());
        close(fd);
    }

    void printClassFile(int level, const std::string &logtxt)
    {
        std::string filename = LogFile;
        filename += ".";
        filename += LevelToString(level);
        printOneFile(filename, logtxt);
    }

    void operator()(int level, const char *format, ...)
    {
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);
        char leftbuffer[SIZE];
        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", LevelToString(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
        va_list s;
        va_start(s, format);
        char rightbuffer[SIZE];
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
        va_end(s);

        // 格式：默认部分+自定义部分
        char logtxt[SIZE * 2];
        snprintf(logtxt, sizeof(logtxt), "%s %s", leftbuffer, rightbuffer);
        // printf("%s", logtxt);
        printLog(level, logtxt);
    }

    ~Log() {}

private:
    /* data */
    int printMethod;
    std::string path;
};
