#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <vector>

__thread pid_t id = 0;

void *routine(void *argc)
{
    ++id;
    while (1)
    {
        printf("new thread %d %p\n", id, &id);
        sleep(1);
    }
    return (void *)"yes!";
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, routine, (void *)"");
    while (1)
    {
        printf("old thread %d %p\n", id, &id);
        sleep(1);
    }
    return 0;
}

// void *routine(void *argc)
// {
//     while (1)
//     {
//         std::cout << (char *)argc << std::endl;
//         sleep(1);
//         break;
//     }
//     return nullptr;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, routine, (void *)"thread-1");
//     void* retval=nullptr;
//     int n=pthread_join(tid,&retval);
//     if(n==0){
//         std::cout<<"等待成功"<<retval<<std::endl;
//     }
//     return 0;
// }

// // std::vector<pthread_t> q2;

// // void* routine(void* pa){
//     std::string q1=(char*)pa;
//     while(1){
//     std::cout<<q1<<std::endl;
//     for(auto&e:q2){
//         std::cout<<e<<std::endl;
//     }
//     sleep(1);
//     }
// }

// int main(){
//     const int a=10;
//     for(int b=0;b<a;++b){
//         pthread_t i;
//         char name[64];
//         snprintf(name,sizeof(name),"new thread-%d\0",b+1);
//         pthread_create(&i,nullptr,routine,(void*)name);
//         q2.push_back(i);
//     }
//     while(1){

//     }
//     // pthread_t tid;
//     // int n=pthread_create(&tid,nullptr,routine,(void*)"hahahya");
//     // while(1){
//     //     std::cout<<"我是老线程,新线程的tid="<<tid<<std::endl;
//     //     sleep(1);
//     // }
//     return 0;
// }
