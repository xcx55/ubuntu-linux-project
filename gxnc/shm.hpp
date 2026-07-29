#pragma once
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <cerrno>
#include <cstdlib>
#include<unistd.h>

const int gsize = 4096;
// ftok()参数
#define PATHNAME "/tmp"
const int p_jd = 0x66;
class shm
{
public:
    shm()
        : _shmid(-1), _size(gsize),_start_add(nullptr)
    {
    }
    
    key_t Get1()
    {
        return ftok(PATHNAME, p_jd);
    }
    void Delete(){
        std::cout<<std::endl;
        std::cout<<"_shmid:"<<_shmid<<std::endl;
        int n=shmctl(_shmid,IPC_RMID,nullptr);
        if(n==-1){
            std::cout<<"删除失败了!"<<"errno:"<<errno<<std::endl;
        }
    }
    void Attach(){
        void* ptr=shmat(_shmid,nullptr,0);
        if((long long int)ptr==-1){
            std::cerr<<"shmat失败"<<" "<<"errno"<<errno<<std::endl;
            exit(3);
        }
        _start_add=ptr;
    }
    void detach(){
        int n=shmdt(_start_add);
    }
    void Get(){
        Creat(IPC_CREAT);
    }
    void Creatnc(){
        Creat(IPC_CREAT|IPC_EXCL|0666);
    }
    void* add(){
        return this->_start_add;
    }
private:
void Creat(int shmflag)
    {
        //1、构建键值
        key_t key = Getkey();
        if (key < 0)
        {
            std::cerr << "key创建失败" << errno << std::endl;
            exit(1);
        }
        std::cout << "这个共享内存的key是:" << key << std::endl;
        //2、创建新的共享内存！
        int shmid = shmget(key, _size, shmflag); // 返回的是 我们传入的key！！
        if (shmid == -1)
        {
            std::cerr << "shmget函数出错! " << "errno:" << errno << std::endl;
            exit(1);
        }
        _shmid=shmid;
        std::cout<<"key="<<Getkey()<<"shmid="<<_shmid<<std::endl;
    }
    key_t Getkey()
    {
        return ftok(PATHNAME, p_jd);
    }

private:
    int _shmid;
    int _size;
    void* _start_add;/////
};