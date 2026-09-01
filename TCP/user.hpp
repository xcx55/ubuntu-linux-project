#ifndef _USER
#define _USER
#pragma once
#include<string>
#include <cstdint>
#include <netinet/in.h>
#include<string.h>
namespace _USER_SPACE
{
    class user
    {
    public:
    user(uint16_t port=1,uint32_t ip=1)
    :_port(port)
    ,_ip(ip)
    {
       
    }
    user(int i,uint16_t port=1,uint32_t ip=1){
        if(i==0) return;
        _port=htons(_port);
        _ip==htonl(_ip);
    }
    uint16_t Getport(){
        return (htons(_port));
    }
    uint32_t Getip(){
        return (htonl(_ip));
    }
    void send(int fd,char* inbuff){
        sockaddr_in pq;
        pq.sin_family=AF_INET;
        pq.sin_port=htons(_port);
        pq.sin_addr.s_addr=htonl(_ip);
        sendto(fd,inbuff,strlen(inbuff),0,(sockaddr*)&pq,sizeof(sockaddr_in));
    }
    bool operator==(user& x){
        return ((_port == x._port) && (_ip == x._ip));
    }
    private:
        uint16_t _port;
        uint32_t _ip;
    };
}

#endif //_USER