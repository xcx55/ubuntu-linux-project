#pragma once
#include<iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstdint>
#include <cstring>
#include<stdio.h>
int main(int argc,char* argv[]){
    if(argc!=3){
        exit(2);
    }
    std::string server_ip=argv[1];
    uint16_t port=std::stoi(argv[2]);
    //1、创建socket文件
    int _sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(_sockfd<0){
        std::cerr<<"socket etton"<<std::endl;
        exit(0);
    }
    struct sockaddr_in out;
    memset(&out,0,sizeof(out));
    out.sin_family=AF_INET;
    out.sin_port=htons(port);
    out.sin_addr.s_addr=inet_addr(server_ip.c_str());
    //2、需要IP和port
    std::string buff;
    char getbuff[1024];
    while(true){
        std::cout<<"please enter#";
        getline(std::cin,buff);//getline函数是给string使用的！
        ssize_t n=sendto(_sockfd,buff.c_str(),buff.size(),0,(sockaddr*)&out,sizeof(out));
        struct sockaddr_in temp;
        socklen_t ilen=sizeof(temp);
        ssize_t h=recvfrom(_sockfd,getbuff,1023,0,(sockaddr*)&temp,&ilen);
        getbuff[h]=0;
        std::cout<<getbuff<<std::endl;
    }
    return 0;
}