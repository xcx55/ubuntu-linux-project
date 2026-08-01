#include"shm.hpp"

int main(){
    shm server;
    server.Getshm();
    char* pa=(char*)server.Attach();
    int index=0;
    while(1){
        char ch;
        std::cin>>ch;
        *(pa+index++)=ch;
        index%=4096;
        if(index==100)
        break;
    }
    server.Detach();
    server.shmdelete();
    return 0;
}