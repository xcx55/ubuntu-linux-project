#ifndef _MUTEX
#define _MUTEX
#pragma once
#include <iostream>
// #include <mutex>
#include <pthread.h>

namespace _MUTEX_SPACE
{
    class Mutex
    {
    public:
        Mutex(){
            pthread_mutex_init(&_lock,nullptr);
        }
        void lock(){
            pthread_mutex_lock(&_lock);
        }
        void unlock(){
            pthread_mutex_unlock(&_lock);
        }
        pthread_mutex_t* Getlock(){
            return &_lock;
        }
        ~Mutex(){
            pthread_mutex_destroy(&_lock);
        }
    private:
    pthread_mutex_t _lock;
    };
}

#endif