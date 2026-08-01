#pragma once
#include<iostream>
#include<unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/ipc.h>
#define PATHNAME "/tmp"
#define p_jd 0x66
class shm{
public:
shm()
:_shmid(-1),
_start_add(nullptr),
_size(4096)
{

}
//申请共享内存物理地址
void Getshm(){
    int n=shmget(ftok(PATHNAME,p_jd),_size,IPC_CREAT|IPC_EXCL|0666);
    if(n==-1){
        std::cerr<<"shmget errno:"<<errno<<std::endl;
        exit(1);
    }
    _shmid=n;
}
//删除打上标签函数
void shmdelete(){
    int n=shmctl(_shmid,IPC_RMID,nullptr);
}
//挂载函数
void* Attach(){
    void* ps=shmat(_shmid,nullptr,0);
    if((long long)ps==-1){
        std::cerr<<"shmat errno:"<<errno<<std::endl;
        exit(2);
    }
    _start_add=ps;
    return ps;
}
//去载函数
void Detach(){
    int n=shmdt(_start_add);
}

//打印信息
void Addr(){
    
}

/////////////////find到了之后  就是挂在！！
void findshm(){
    int n=shmget(ftok(PATHNAME,p_jd),_size,IPC_CREAT);
    if(n==-1){
        std::cerr<<"shmget errno:"<<errno<<std::endl;
        exit(1);
    }
    _shmid=n;
}
private:
int _shmid;
int _size;
void* _start_add;
};