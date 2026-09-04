#ifndef _COND
#define _COND
#include <pthread.h>
#include"Mutex.hpp"
using namespace _MUTEX_SPACE;
namespace _COND_SPACE{
    class Cond{
        public:
        Cond(){
            pthread_cond_init(&cond, nullptr);
        }
        void Wait(Mutex* pa){
            pthread_cond_wait(&cond,pa->Getlock());
        }
        void signal(){
            pthread_cond_signal(&cond);
        }
        void broadcast(){
            pthread_cond_broadcast(&cond);
        }
        ~Cond(){
            pthread_cond_destroy(&cond);
        }
        private:
        pthread_cond_t cond;
    };
}

#endif 