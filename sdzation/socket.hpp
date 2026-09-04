#ifndef _SOCKET_SPACE
#define _SOCKET_SPACE
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
#include <string>
#include <pthread.h>
#include <memory>
#include"logger.hpp"
namespace _SOCKET
{
    enum EXIT
    {
        socketfail,
        binderror,
        listenerror,
        accepterror

    };
    enum class LogLevel

    {

        INFO,

        WARNING,

        ERROR,

        FATAL,

        DEBUG

    };
    
    class Socket
    {
    public:
        ~Socket()
        {
        }

    public:
        virtual void CreatSocketOrDie() = 0;
        virtual void BindSocketOrDie() = 0;
        virtual void ListenSocketOrDie() = 0;
        virtual ssize_t Recv(std::string *in) = 0;
        virtual ssize_t Send(const std::string& out) = 0;
        virtual std::shared_ptr<Socket> Accept(sockaddr_in *ps) = 0;
        virtual bool Havefd() = 0;
        virtual int Sockfd() = 0;
        virtual void Close() = 0;
        virtual void BulidTcpclient()=0;
        virtual void Connect()=0;
    public:
        void BulidTcpSocketMathod()
        {
            CreatSocketOrDie();
            BindSocketOrDie();
            ListenSocketOrDie();
        }
        void BulidUdpSocketMathod()
        {
            CreatSocketOrDie();
            BindSocketOrDie();
        }

    private:
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(char *asg)
            : Socket()
        {
            hostaddr.sin_port = (atoi(asg));
            hostaddr.sin_addr.s_addr = INADDR_ANY;
            hostaddr.sin_family = AF_INET;
            netaddr.sin_port = htons(atoi(asg));  // 网络字节序，要给另外一台主机！
            netaddr.sin_addr.s_addr = INADDR_ANY; // 宏不需要转
            netaddr.sin_family = AF_INET;         // 交给内核！
        }
        void BulidTcpclient()override{
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                log << "[fatal] TCP create sockfd , err is" << strerror(errno) << '\n';
                exit(socketfail);
            }
            else
            {
                log << "[info] TCP creat sucuess" << '\n';
            }
        }
        void Connect(){
            connect();
        }

        TcpSocket(int sockfd)
        {
            _sockfd = sockfd;
        }
        TcpSocket(char* pa,char* pb){
            inet_pton(AF_INET,pa,&netaddr.sin_addr);
            netaddr.sin_port=std::stoi(pb);
            netaddr.sin_family=AF_INET;
        }
        void CreatSocketOrDie()override
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                log << "[fatal] TCP create sockfd , err is" << strerror(errno) << '\n';
                exit(socketfail);
            }
            else
            {
                log << "[info] TCP creat sucuess" << '\n';
            }
        }
        void BindSocketOrDie()
        {
            if (bind(_sockfd, (sockaddr *)&netaddr, sizeof(sockaddr_in)) != 0)
            {
                log << "[error] TCP bind error" << '\n';
                exit(binderror);
            }
            else
            {
                log << "[info] TCP bind success" << '\n';
            }
        }
        void ListenSocketOrDie()
        {
            if (listen(_sockfd, 0) != 0)
            {
                log << "[error] TCP listen error" << '\n';
                exit(listenerror);
            }
            else
            {
                log << "[info] TCP listen success" << '\n';
            }
        }
        int Sockfd()
        {
            return _sockfd;
        }
        std::shared_ptr<Socket> Accept(sockaddr_in *ps)//这里是我觉得最精妙的地方！
        {
            socklen_t a = sizeof(sockaddr_in);
            sockaddr_in pa;
            int n = accept(_sockfd, (sockaddr *)&pa, &a); // 不知道内核干了什么 好不舒服
            if (n < 0)
            {
                log << "[error] accept error" << '\n';
                exit(accepterror);
            }
            ps->sin_port = ntohs(pa.sin_port);
            ps->sin_addr.s_addr = ntohl(pa.sin_addr.s_addr);
            ps->sin_family = pa.sin_family;
            _sockfd = n;
            return std::make_shared<TcpSocket>(_sockfd); // 返回指向TcpSocket表的指针
        }
        bool Havefd()
        {
            return _sockfd != 0;
        }
        ssize_t Recv(std::string *in)
        {
            char inbuff[1024];
            ssize_t n = recv(_sockfd, inbuff, sizeof(inbuff) - 1, 0);//这里是接收报文数据
            if (n > 0)
            {
                inbuff[n] = 0;
                (*in) += inbuff;///这里方便了我们的解析！一直变长 进行拼凑！
            }
            return n;
        }
        ssize_t Send(const std::string& out)
        {
            ssize_t n = send(_sockfd, out.c_str(), out.size(), 0);//返回的 
            //一定是由\0的 但是size不会计入\0 ，进行截断！
            // out.clear();
            return n;
        }
        void Close()
        {
            if (_sockfd > 0)
            {
                close(_sockfd);
                _sockfd=0;
            }
        }

    private:
        // int _listensockfd;
        int _sockfd = 0;
        sockaddr_in netaddr;
        sockaddr_in hostaddr;
    };
    class UdpSocket : public Socket
    {
    };
}

#endif //_SOCKET_SPACE