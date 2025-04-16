#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " serverip serverport\n"
              << std::endl;
}

struct PthreadData
{
    struct sockaddr_in server;
    int sockfd;
    std::string serverip;
};

void *recv_message(void *args)
{
    PthreadData *td = static_cast<PthreadData *>(args);
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);

        ssize_t s = recvfrom(td->sockfd, buffer, 1023, 0, (struct sockaddr *)&temp, &len);
        if (s > 0)
        {
            buffer[s] = 0;
            cerr << buffer << endl;
        }
    }
}

void *send_message(void *args)
{
    PthreadData *td = static_cast<PthreadData *>(args);
    string message;
    socklen_t len = sizeof(td->server);

    std::string welcome = td->serverip;
    welcome += " comming...";
    sendto(td->sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&(td->server), len);

    while (true)
    {
        cout << "Please Enter@ ";
        getline(cin, message);

        // std::cout << message << std::endl;
        // 1. 数据 2. 给谁发
        sendto(td->sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&(td->server), len);
    }
}

// ./udpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    struct PthreadData td;

    // struct sockaddr_in server;
    bzero(&td.server, sizeof(td.server));
    td.server.sin_family = AF_INET;
    td.server.sin_port = htons(serverport);
    td.server.sin_addr.s_addr = inet_addr(serverip.c_str());
    // socklen_t len = sizeof(td.server);

    // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    td.sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (td.sockfd < 0)
    {
        cout << "socker error" << endl;
        return 1;
    }

    td.serverip = serverip;

    // client 要bind吗？要！只不过不需要用户显示的bind！一般有OS自由随机选择！
    // 一个端口号只能被一个进程bind，对server是如此，对于client，也是如此！
    // 其实client的port是多少，其实不重要，只要能保证主机上的唯一性就可以！
    // 系统什么时候给我bind呢？首次发送数据的时候

    pthread_t recvr, sender;
    pthread_create(&recvr, nullptr, recv_message, &td);
    pthread_create(&sender, nullptr, send_message, &td);

    // string message;
    // char buffer[1024];
    // while (true)
    // {
    //     // cout << "Please Enter@ ";
    //     // getline(cin, message);

    //     // // std::cout << message << std::endl;
    //     // // 1. 数据 2. 给谁发
    //     // sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, len);

    //     // struct sockaddr_in temp;
    //     // socklen_t len = sizeof(temp);

    //     // ssize_t s = recvfrom(sockfd, buffer, 1023, 0, (struct sockaddr*)&temp, &len);
    //     // if(s > 0)
    //     // {
    //     //     buffer[s] = 0;
    //     //     cout << buffer << endl;
    //     // }
    // }

    pthread_join(recvr, nullptr);
    pthread_join(sender, nullptr);

    close(td.sockfd);
    return 0;
}