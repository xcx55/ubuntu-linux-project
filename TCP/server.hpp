#ifndef _SERVER_SPACE
#define _SERVER_SPACE

#include <sys/socket.h> // socket(),bind(),listen(),accept()，套接字基础API、结构体
#include <netinet/in.h> // sockaddr_in，IP协议、端口宏，网络字节序相关结构体
#include <arpa/inet.h>  // inet_pton()、inet_ntop()，IP地址字符串与二进制转换
#include <unistd.h>     // close()，文件描述符关闭
#include <string.h>     // memset()，结构体清零
#include <stdio.h>      // printf/perror
#include <errno.h>      // errno错误码
#include <iostream>
#include <cstdlib>
#include "user.hpp"
#include <signal.h>
#include "thread.hpp"
#include "threadpool.hpp"
using namespace _USER_SPACE;
enum exitnm
{
    _usag,
    _bind,
    _listen
};
namespace _SERVER
{
    class TCPserver
    {
    public:
        TCPserver(const char *a)
            : _port(std::stoi(a))
        {
        }
        void init()
        {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            sockaddr_in pq;
            pq.sin_port = htons(_port);
            pq.sin_addr.s_addr = INADDR_ANY;
            pq.sin_family = AF_INET;
            int n = bind(sockfd, (sockaddr *)&pq, sizeof(pq));
            if (n < 0)
            {
                std::cout << "bind绑定文件socket失败!" << std::endl;
                exit(_bind);
            }
            else
            {
                std::cout << "bind success" << std::endl;
            }
            int s = listen(sockfd, 16);
            if (s < 0)
            {
                std::cout << "listen监听失败!" << std::endl;
                exit(_listen);
            }
            else
            {
                std::cout << "listem success" << std::endl;
            }
            _listensockfd = sockfd;
        }

        void ServerIO(int fd, sockaddr_in pq)
        {
            while (true)
            {
                std::cout << fd << " " << std::endl;
                {
                    char buff[1024];
                    int n = read(fd, buff, sizeof(buff) - 1); // 这一版有个问题 server会阻塞等待客户端
                    if (n < 0)
                    {
                        std::cout << "read err" << strerror(errno) << std::endl;
                        break;
                    }
                    else if (n == 0)
                    {
                        std::cout << "读取完毕" << std::endl;
                        break;
                    }
                    else
                    {
                        buff[n] = 0;
                        sockaddr_in p;

                        std::cout << "client say#" << "[" << inet_ntoa(pq.sin_addr)
                                  << ":" << ntohs(pq.sin_port) << "]" << buff << std::endl;
                        write(fd, buff, sizeof(buff) - 1);
                    }
                }
            }
            close(fd);
        }
        using callback_t = std::function<void()>;
        void Start()
        {
            using namespace _THREADPOOL_SPACE;
            signal(SIGCHLD, SIG_IGN);
            threadpool<callback_t>* pa = threadpool<callback_t>::Getthreadpool();
            while (true)
            {
                sockaddr_in pq;
                socklen_t a = sizeof(sockaddr_in);
                int fd = accept(_listensockfd, (sockaddr *)&pq, &a); // listen内部维护一个加锁队列？
                if (fd < 0)
                { // accept没有读取到  就会阻塞！而不是返回负数
                    std::cout << "accept err" << std::endl;
                    continue;
                }
                // pid_t id = fork();//version 2  多进程
                // if (id > 0)
                // {
                //     close(_listensockfd);//引用计数
                //     ServerIO(fd, pq);
                //     exit(0);
                // }
                // close(fd);
                // version 3 多线程
                // Thread *ac = new Thread([fd, pq, this]()
                //                         { this->ServerIO(fd, pq); }); // lambda表达式
                // // 内部的变量名是一样的
                // ac->start();
                // ac->Detach();
                // version4  接入线程池
                pa->Enqueue([fd,pq,this](){this->ServerIO(fd,pq);});
            }
        }

    private:
        int _port;
        int _listensockfd;
    };

}

#endif //_SERVER_SPACE