#ifndef _TCPSERVER_SPACE
#define _TCPSERVER_SPACE
#pragma once
#include "socket.hpp"
#include <memory>
#include <unistd.h>
#include "threadpool.hpp"
#include <signal.h>
#include <functional>
using namespace _THREADPOOL_SPACE;
using namespace _SOCKET;
using callback_t = std::function<std::string(std::string &)>;
std::string net_to_host(sockaddr_in *pa)
{
    char addr[1024];
    std::string pq = inet_ntop(AF_INET, &pa->sin_addr, addr, sizeof(addr) - 1);
    int port = ntohs(pa->sin_port);
    return pq + ":" + std::to_string(port);
}
class Tcpserver
{
public:
    Tcpserver(callback_t hander, char *asg)
    {
        _cb = hander;
        _port = (atoi(asg));
        _sockptr = std::make_unique<TcpSocket>(asg);
        _sockptr->BulidTcpSocketMathod();
    }
    Tcpserver()
    {
    }
    void Loop()
    {
        signal(SIGCHLD, SIG_IGN);
        while (true)
        {
            sockaddr_in ps;
            auto &&p = _sockptr->Accept(&ps);
            if (!p->Havefd())
            {
                log << "[debug] not have fd,again" << '\n';
                continue;
            }
            char client_ip[INET_ADDRSTRLEN]{};
            inet_ntop(AF_INET, &ps.sin_addr, client_ip, sizeof(client_ip));
            log << "[info] get a new link:" << client_ip << ':'
                << ps.sin_port << "port:" << p->Sockfd() << '\n';
            if (fork() == 0)
            { /////版本1 进程
                // 子进程，一些服务！
                service(p, ps);
                p->Close();
                exit(0);
            }
            // 线程池
            //  _pool->Enqueue([p,ps](){Tcpserver q;
            //      q.service(p,ps);});
        }
    }

private:
    void service(std::shared_ptr<Socket> &a, sockaddr_in &user)
    {
        std::string in, out;
        while (true)
        {
            // in.clear();
            out.clear();
            int n = a->Recv(&in);
            // log<<"[info]"<<fa.c_str()<<'\n';
            if (n <= 0)
            {
                log << "[error] Recv error or client exit" << net_to_host(&user) << '\n';
                break;
            }
            /////////////
            if (_cb)
                out = _cb(in); // 这里包含了很多逻辑！
            if (out.empty())
                continue;
            n = a->Send(out);
            if (n <= 0)
            {
                log << "[error] Recv error or client exit" << net_to_host(&user) << '\n';
                break;
            }
        }
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _sockptr;
    // inline static threadpool<callback_t> *_pool = threadpool<callback_t>::Getthreadpool();
    callback_t _cb;
};

#endif //_TCPSERVER_SPACE
