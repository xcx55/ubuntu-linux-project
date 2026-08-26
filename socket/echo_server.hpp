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
    Udpserver(std::string &p, uint16_t port)
        : _sockfd(-1),
          _ip(p),
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
        struct sockaddr_in local;
        bzero(&local,sizeof(sockaddr_in));
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);
        local.sin_addr.s_addr=inet_addr(_ip.c_str());

        int n=bind(_sockfd,(sockaddr*)&local,sizeof(local));
    }
    void loop() {}

private:
    int _sockfd;
    std::string _ip;
    uint16_t _port;
};

#endif