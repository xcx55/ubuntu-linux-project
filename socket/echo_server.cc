#pragma once
#include"echo_server.hpp"
#include<memory>
int main(int argc,char* argv[]){
    if(argc!=2){
        exit(usag_erron);
    }
    uint16_t port=std::stoi(argv[1]);
    std::unique_ptr<Udpserver> usr=std::make_unique<Udpserver>(port);
    usr->init();
    usr->start();
    return 0;
}