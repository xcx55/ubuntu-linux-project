#include"server.hpp"
using namespace _SERVER;



void Usag(){
    std::cout<<"./exe port号"<<std::endl;
    exit(_usag);
}

int main(int argc,char* argv[]){
    if(argc!=2){
        Usag();
    }
    
    TCPserver pa(argv[1]);
    pa.init();
    pa.Start();
    return 0;
}

