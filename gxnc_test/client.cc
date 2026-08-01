#include"shm.hpp"

int main(){
    shm client;
    client.findshm();
    char* pa=(char*)client.Attach();
    int index=0;
    while(1){
        std::cout<<"开始了"<<std::endl;
        std::cout<<*(pa+index++)<<std::endl;
        sleep(2);
        if(index==100)
        break;
    }
    client.Detach();
    client.shmdelete();
    return 0;
}