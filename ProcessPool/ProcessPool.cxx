#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
// 子进程完成任务代码
using task_t = std::function<void()>;

void Syncdisk()
{
    std::cout << "将数据刷入磁盘" << std::endl;
    sleep(1);
}

void Download()
{
    std::cout << "下载数据到磁盘" << std::endl;
    sleep(1);
}

void Printlog()
{
    std::cout << "打印日志到了显示器" << std::endl;
    sleep(1);
}

void Aptstatus()
{
    std::cout << "更新我们的状态" << std::endl;
    sleep(1);
}
// 任务列表
task_t tasks[4] = {Syncdisk, Download, Printlog, Aptstatus}; // 函数只能靠类型来集合！ 只能使用广义类型 函数名不一样都可以！

const int gprocessnum = 5;
using cb_t = std::function<void(int)>;

enum
{
    OK = 0,
    PIPE_ERR = 1,
    FORK_ERR = 2,
    indexerr = 3
};

void Dotask(int fd)
{
    // std::cout<<"进入了Dotask函数"<<std::endl;
    // sleep(1000);
    // sleep(100);
    while (1)
    {
        int task_code = 0;
        ssize_t n = read(fd, &task_code, sizeof(task_code)); // 对于管道  我们read write机制和普通文件不同！
        // std::cout<<"read完成!"<<std::endl;
        if (n == sizeof(task_code) && task_code < 4)
        {
            tasks[task_code](); // 执行任务码任务!
            // std::cout<<"执行了"<<std::endl;
        }
        else if (n == 0)
        {
            printf("task exit:because 写端结束，读端读完了\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }
}

class Channel
{
public:
    Channel(int wfd, int sub_pid)
        : _wfd(wfd), _sub_pid(sub_pid)
    {
        _sub_name = "-sub-process-" + std::to_string(_sub_pid);
    }
    void PrintfInfo()
    {
        // 打印这个类的数据
        printf("wfd权柄:%d,当前管道权柄所链接子进程pid:%d,子进程的名字:%s\n", this->_wfd, _sub_pid, _sub_name.c_str());
    }
    void Write(int itask)
    {
        ssize_t n = write(_wfd, &itask, 4); // 约定4字节的发送！写 也可以是字节指针！！int的也可以
    }
    const char *Name() const
    {
        return _sub_name.c_str();
    }
    pid_t Getpid()
    {
        return this->_sub_pid;
    }
    void Closewfd()
    {
        close(_wfd);
    }
    ssize_t Getfd()
    {
        return _wfd;
    }

private:
    int _wfd;              // 管道的文件描述符
    pid_t _sub_pid;        // 进程的pid
    std::string _sub_name; // 进程的名字
    // int cnt;               //计数器
};

// 进程池的类
class ProcessPool
{
public:
    ProcessPool()
    {
        srand((unsigned int)time(NULL)); // 种下随机种子}
    }
    ~ProcessPool() {};
    // 打印channels信息
    void Debug()
    {
        for (auto &e : channels)
        {
            e.PrintfInfo();
        }
    }

    // 创建进程池
    void init(cb_t cb)
    {
        this->CreatProcessChannel(cb);
    }

    // run
    void run()
    {
        int cnt = 10;
        while (cnt--)
        {
            // 选择一个管道下标！
            int who = Selecchannelindex();
            sleep(1);
            // 选择一个任务
            int task = Selecttask();
            sleep(1);
            // printf("马上执行\n");
            // 发送一个任务
            Sendtasktoprocess(who, task);
            printf("正在发送%d号任务到%d管道%s\n", task, who, channels[task].Name());
        }
    }

    void quit()
    {
        // version2
        for (auto &e : channels)
        {
            e.Closewfd();
            waitpid(e.Getpid(), nullptr, 0);
            std::cout<<"等待"<<e.Getpid()<<"进程成功!"<<std::endl;
        }

        // version1// 全部关完了再来等待
        //  for(auto& e:channels){
        //      e.Closewfd();
        //  }
        //  for(auto& e:channels){
        //      pid_t id=e.Getpid();
        //      waitpid(id,nullptr,0);
        //      std::cout<<"子进程"<<id<<":等待回收成功!"<<std::endl;
        //  }
    }

private:
    void Sendtasktoprocess(int index, int itask)
    {
        if (index < 0 || index > 5 || itask < 0 || itask > 4)
        {
            printf("index=%d,itask=%d", index, itask);
            exit(indexerr);
        }
        channels[index].Write(itask);
    }
    int Selecttask()
    {
        int a = rand() % 4;
        return a;
    }
    int Selecchannelindex()
    {
        static int index = 0;
        index = (++index) % channels.size();
        return index;
    }
    void CreatProcessChannel(cb_t cb) // 仿函数
    {
        for (int i = 0; i < gprocessnum; i++)
        {
            int pipefd[2];
            int n = pipe(pipefd);
            if (n < 0)
            {
                std::cerr << "pipe" << std::endl;
                exit(PIPE_ERR);
            }
            pid_t id = fork();
            if (id < 0)
            {
                std::cerr << "fork err" << std::endl;
                exit(FORK_ERR);
            }
            else if (id == 0)
            { // 子进程
                // do thing
                // 关闭历史fd！！
                if (channels.size() > 0)//不会有时序问题吗？假设先是这行代码执行  再push_back()//1、>0 2、时序问题！
                {
                    for(auto& e:channels){
                        e.Closewfd();
                        printf("guanbing历史fd\n");
                    }
                }
                close(pipefd[1]);
                cb(pipefd[0]);
                exit(OK);
            }
            else
            { // 父进程
              // do thing
            }
            close(pipefd[0]);
            // 接收我们的父进程的写入fd权柄以及信息
            // Channel ch({pipefd[1], id});
            // channels.push_back(ch);
            channels.emplace_back(pipefd[1], id);
            // sleep(1);
            std::cout << "子进程创建中" << std::endl;
        }
    }
    std::vector<Channel> channels;
};

int main()
{ // 初始化进程池
    ProcessPool pp;
    pp.init(Dotask);
    // 2、父进程控制子进程！
    pp.run();
    // 打印
    pp.Debug();
    // 3、释放所有资源，包括用户态空间 以及 PCB内核空间！
    //  sleep(1000);
    pp.quit();

    return 0;
}