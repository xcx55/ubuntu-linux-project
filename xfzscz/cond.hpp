#pragma once
#ifndef COND_HPP
#define COND_HPP
// cond.hpp 开头

template<typename T>
class blackqueue;   // 模板前向声明，只告诉编译器有这个类，不需要完整定义
#include <queue>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "queue.hpp"
template<class T>
class cond
{
public:
    cond()
    {
        pthread_cond_init(&condition, nullptr);
    }
    void waitcond(blackqueue<T>* a)
    {
        int i = 1;
        while (i)
        {
            i = pthread_cond_wait(&condition, a->Getmutex());
        }
    }
    void signalcond(){
        int i=1;
        while(i){
            i=pthread_cond_signal(&condition);
        }
    }
    void broadcastcond(){
        int i=1;
        while(i){
            i=pthread_cond_broadcast(&condition);
        }
    }
    ~cond(){
        pthread_cond_destroy(&condition);
    }
private:
    pthread_cond_t condition;
};

#endif // COND_HPP
