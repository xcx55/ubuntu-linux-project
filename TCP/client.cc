#include <sys/socket.h> // socket(),bind(),listen(),accept()，套接字基础API、结构体
#include <netinet/in.h> // sockaddr_in，IP协议、端口宏，网络字节序相关结构体
#include <arpa/inet.h>  // inet_pton()、inet_ntop()，IP地址字符串与二进制转换
#include <unistd.h>     // close()，文件描述符关闭
#include <string.h>     // memset()，结构体清零
#include <stdio.h>      // printf/perror
#include <errno.h>      // errno错误码
#include <iostream>
#include <cstdlib>
#include <csignal>
// #include "user.hpp"
// using namespace _USER_SPACE;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(0);
    }
    std::string server_ip = argv[1];
    uint16_t _port = std::stoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cout << "socket err" << std::endl;
        exit(0);
    }
    uint32_t ip = inet_addr(server_ip.c_str());
    uint16_t port = htons(_port);
    sockaddr_in p;
    p.sin_port = port;
    p.sin_family = AF_INET;
    p.sin_addr.s_addr = ip;
    int n = connect(sockfd, (sockaddr *)&p, sizeof(p)); // 绑定连接一起！ UDP直接就是发送！
    if (n < 0)
    {
        std::cerr << "connet to server erron:" << strerror(errno) << std::endl;
        exit(3);
    }
    else
    {
        std::cout << "connect to server success" << std::endl;
    }

    while (true)
    {
        std::string buff;
        char inbuff[1024];
        std::cout << "please enter#";
        getline(std::cin, buff);
        int s = write(sockfd, buff.c_str(), buff.size());
        if (s < 0)
        {
            std::cout << "对面没了" << std::endl;
            exit(4);
        }
        else if (s == 0)
        {
            std::cout << "arrive end of file" << std::endl;
            break;
        }
        else
        {
            std::cout << "发送成功" << std::endl;
        }
        int g = read(sockfd, inbuff, sizeof(buff) - 1);
        if (g < 0)
        {
            std::cout << "read err" << std::endl;
            break;
        }
        else if (g == 0)
        {
            std::cout << "写端(服务端）没了,arrive end of file" << std::endl;
            break;
        }
        else
        {
            inbuff[g] = 0;
            // std::cout<<"server say#"<<inbuff<<std::endl;
            std::cout << "server say#" << "[" << inet_ntoa(p.sin_addr)
                      << ":" << ntohs(p.sin_port) << "]" << buff << std::endl;
        }
    }
    return 0;
}