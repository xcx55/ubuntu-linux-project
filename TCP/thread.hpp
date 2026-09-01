#ifndef _THREAD
#define _THREAD

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <functional>
using callback_t = std::function<void()>;
class Thread
{
public:
    Thread(callback_t cb)
    :_cb(cb)
    {}
    void start(){
        pthread_create(&id,nullptr,routine,this);//call addr
    }
    void Join(){
        pthread_join(id,nullptr);
    }
    void Detach(){
        pthread_detach(id);
    }
    void Close(){

    }
private:
static void* routine(void* ta){
    // Thread* pa=new Thread(nullptr);
    Thread* pa=static_cast<Thread*>(ta);
    pa->_cb();
    delete pa;
    return nullptr;
}
    callback_t _cb;
    pthread_t id;
};

#endif //_THREAD