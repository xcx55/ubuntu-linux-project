#include "shm.hpp"
#include <unistd.h>

int main()
{
    shm sharememory;
    sharememory.Creatnc(); // 创建一个裸体内存
    // sharememory.Delete();
    sharememory.Attach(); // 建立联系，引用计数+1，加上共享内存虚拟地址
    system("ipcs -m");
    sleep(2);
    int index = 0;
    char *ps = (char *)sharememory.add();
    printf("%p\n",ps);
    while (1)
    {
        index=0;
        while (index < 10)
        {
            std::cout<<"Please enter@"<<std::endl;
            char ch;
            std::cin >> ch;
            ps[index++] = ch;
            // index%=10;
            sleep(2);
        }
    }
    sharememory.detach(); // 接触引用计数！
    system("ipcs -m");
    sharememory.Delete(); // 控制函数打上脏页 1、脏页2、引用计数为0 OS识别直接删除
    // std::cout << "delete done\n";
    // usleep(500*1000); //休眠500ms
    system("ipcs -m");
    sleep(2);
    return 0;
}