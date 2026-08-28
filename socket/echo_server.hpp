#ifndef _ECHO_SERVER
#define _ECHO_SERVER
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

enum exit
{
    sock_erron,
    usag_erron
};

class Udpserver
{
public:
    Udpserver(uint16_t port)
        : _sockfd(-1),
        //   _ip(p),
          _port(port)
    {
    }
    ~Udpserver() {}
    void init()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            std::cerr << "socket error" << std::endl;
            exit(sock_erron);
        }
        else
        {
            std::cout << "create socket file success" << std::endl;
        }
        struct sockaddr_in local;
        bzero(&local, sizeof(sockaddr_in));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        // local.sin_addr.s_addr=inet_addr(_ip);
        int n = bind(_sockfd, (sockaddr *)&local, sizeof(local));
        if (n == 0)
        {
            std::cout << "bind success" << std::endl;
        }
        else
        {
            std::cout << "bind errno" << std::endl;
            exit(0);
        }
    }
    void start()
    {
        while (true)
        {
            char inbuff[1024];
            sockaddr_in in;
            socklen_t len = sizeof(in);
            // 需要用户套接字信息！
            int n = recvfrom(_sockfd, inbuff, sizeof(inbuff) - 1, 0, (sockaddr *)&in, &len);
            inbuff[n] = 0;
            int port = ntohs(in.sin_port);
            std::string pq = inet_ntoa(in.sin_addr);
            std::string p1 = "[" + pq + ":" + std::to_string(port) + "#" + "]";
            if (n > 0)
            {
                std::cout << "client say#" << p1 << inbuff << std::endl;
                std::string server_echo = "server say#";
                server_echo += inbuff;
                sendto(_sockfd, server_echo.c_str(), server_echo.size(), 0, (sockaddr *)&in, len);
            }
            else
            {
                std::cerr << "recvfrom erron" << std::endl;
            }
        }
    }
    void loop() {}

private:
    int _sockfd;
    // std::string _ip;
    uint16_t _port;
};

#endif