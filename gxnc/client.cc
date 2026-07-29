#include "shm.hpp"

int main()
{
    shm shmclient;
    shmclient.Get();
    shmclient.Attach();

    char *pa = (char *)shmclient.add();
    printf("%p\n",pa);
    int index = 0;
    while (1)
    {
        index=0;
        std::cout<<"开始进入client的while1循环"<<std::endl;
        while (index < 10)
        {
            
            std::cout << pa[index++] << std::endl;
            
            sleep(2);
        }
    }
    shmclient.detach();
    return 0;
}