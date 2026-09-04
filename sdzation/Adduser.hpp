#ifndef _ADDUSER
#define _ADDUSER
#pragma once
#include <stdio.h>      // printf、perror
#include <stdlib.h>     // exit、malloc
#include <string.h>     // memset、strcpy
#include <unistd.h>     // close、read、write
#include <fcntl.h>      // fcntl 设置非阻塞
#include <sys/socket.h> // socket、bind、listen、accept、connect
#include <netinet/in.h> // sockaddr_in、htons、htonl、INADDR_ANY
#include <arpa/inet.h>  // inet_addr、inet_ntoa、inet_pton、inet_ntop
#include <sys/types.h>  // 基础类型，很多系统调用依赖
#include <errno.h>      // errno 错误码
#include <signal.h>     // 信号处理
#include <sys/wait.h>   // wait、waitpid
#include <iostream>
#include<string>
#include<pthread.h>
namespace _ADDUSER_SPACE
{
    class Adduser
    {
        public:
        Adduser(sockaddr_in& addr)
        :_port(ntohs(addr.sin_port))
        {
            char p[1024];
            ip=inet_ntop(AF_INET,&addr.sin_addr,p,sizeof(p));
        }
        private:
        sockaddr_in ps;
        std::string ip;
        uint16_t _port;
    };
}
#endif //_ADDUSER