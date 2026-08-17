#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>
typedef void (*task_t)();
enum status
{
    thread_new,
    thread_running,
    thread_stop
};
namespace threadmodule
{
    static int number = 1;
    class thread
    {
    private:
        static void Torunning(thread *const pa)
        {
            pa->_status = thread_running;
        }
        static void *routine(void *argc)
        {
            thread *self = (thread *)argc;
            Torunning(self);
            self->_cb();
            return nullptr;
        }
        void Tostop()
        {
            this->_status = thread_stop;
        }

    public:
        thread(task_t a)
            : _tid(0), _status(thread_new), _cb(a)
        {
            _name = std::string("thread-") + std::to_string(1);
            ++number;
        }
        ~thread() {}
        bool start()
        {
            int n = pthread_create(&_tid, nullptr, thread::routine, (void *)this);
            Torunning(this);
            if (n != 0)
            {
                printf("this is error:%d\n", n);
                return false;
            }
            std::cout << this->_tid;
            return true;
        }
        void stop()
        {
            if (_status == thread_running)
            {
                pthread_cancel(_tid);
                this->Tostop();
            }
        }
        bool join()
        {
            if (_joinable)
            {
                int n = pthread_join(_tid, NULL);
                if (n != 0)
                {
                    printf("函数等待失败!\n");
                    return false;
                }
                return true;
            }
            else{
                std::cout<<"join not permitting!"<<std::endl;
            }
        }
        void detch()
        {
            if (_status == thread_running && _joinable)
            {
                pthread_detach(_tid);
                _joinable = false;
            }
            else
            {
                std::cerr << "pthread_detach failed" << std::endl;
            }
        }

    private:
        std::string _name;
        pthread_t _tid;
        int _status;
        bool _joinable = true;
        task_t _cb;
    };
}
