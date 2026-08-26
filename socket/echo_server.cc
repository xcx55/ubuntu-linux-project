#pragma once
#include"echo_server.hpp"
#include<memory>
int main(int argc,char* argv[]){
    if(argc!=3){
        exit(usag_erron);
    }
    std::string ip=argv[1];
    uint16_t port=std::stoi(argv[2]);
    std::unique_ptr<Udpserver> usr=std::make_unique<Udpserver>(ip,port);
    return 0;
}