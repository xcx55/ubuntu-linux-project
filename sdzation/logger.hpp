#ifndef _LOG
#define _LOG
#pragma once
#include<pthread.h>
#include<iostream>

class loger
    {
    public:
        loger()
        {
            pthread_mutex_init(&_lock, nullptr);
        }
        template <class T>
        loger &operator<<(T &&pq)
        {
            pthread_mutex_lock(&_lock);
            std::cout << pq;
            pthread_mutex_unlock(&_lock);
            // std::cout << std::endl;
            return *this;
        }

    private:
        // inline static loger* _ptr=nullptr;单例模式不好实现
        // 由于是指针！*pa<<
        pthread_mutex_t _lock;
    };
    loger log;
    #endif //_LOG