#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>
#include <string>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"

// using func_t = std::function<std::string(const std::string&)>;
typedef std::function<std::string(const std::string &, const std::string &, uint16_t)> func_t;

Log lg;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR
};

uint16_t defaultport = 8080;
std::string defaultip = "0.0.0.0";
const int size = 1024;

class UdpServer
{
public:
    UdpServer(const uint16_t &port = defaultport, const std::string &ip = defaultip)
        : sockfd_(0), port_(port), ip_(ip), isrunning_(false) {}

    void Init()
    {
        // 1. 创建udp socket
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0); // PF_INET
        if (sockfd_ < 0)
        {
            lg(Fatal, "socket create error, sockfd: %d", sockfd_);
            exit(SOCKET_ERR);
        }
        lg(Info, "socket create success, sockfd: %d", sockfd_);
        // 2. bind socket
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port_);                  // 需要保证我的端口号是网络字节序列，因为该端口号是要给对方发送的
        local.sin_addr.s_addr = inet_addr(ip_.c_str()); // 1. string -> uint32_t 2. uint32_t必须是网络序列的 // ??
        // local.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sockfd_, (const struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, errno: %d, err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        lg(Info, "bind success, errno: %d, err string: %s", errno, strerror(errno));
    }

    // 检查是否是新用户
    void CheckUser(const struct sockaddr_in &client,
                   const std::string &clientip, uint16_t clientport)
    {
        auto iter = online_user_.find(clientip);
        // 如果未找到, 将 clientip 添加进 online_user_ 中
        if (iter == online_user_.end())
        {
            online_user_.insert({clientip, client});
            std::cout << "[" << clientip << ":" << clientport << "] add to online user." << std::endl;
        }
    }

    // 发送消息给所有的在线用户
    void BroadCast(const std::string &info, const std::string &clientip,
                   uint16_t clientport)
    {
        for (const auto &user : online_user_)
        {
            std::string message = "[";
            message += clientip;
            message += ":";
            message += std::to_string(clientport);
            message += "]#";
            message += info;
            socklen_t len = sizeof(user.second);
            sendto(sockfd_, message.c_str(), message.size(), 0, (const sockaddr *)(&user.second), len);
        }
    }

    void Run() // 对代码进行分层
    {
        isrunning_ = true;
        char inbuffer[size];
        while (isrunning_)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&client, &len);
            if (n < 0)
            {
                lg(Warn, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }

            // 客户端的ip 和 端口号
            uint16_t clientport = ntohs(client.sin_port);
            std::string clientip = inet_ntoa(client.sin_addr);

            CheckUser(client, clientip, clientport);
            std::string info = inbuffer;
            BroadCast(info, clientip, clientport);

            // inbuffer[n] = 0;
            // std::string echo_string = func(info, clientip, clientport);
            // sendto(sockfd_, echo_string.c_str(), echo_string.size(), 0, (const sockaddr *)&client, len);
        }
    }

    ~UdpServer()
    {
        if (sockfd_ > 0)
            close(sockfd_);
    }

private:
    int sockfd_;     // 网路文件描述符
    std::string ip_; // 任意地址bind 0
    uint16_t port_;  // 表明服务器进程的端口号
    bool isrunning_;
    std::unordered_map<std::string, struct sockaddr_in> online_user_;
};
