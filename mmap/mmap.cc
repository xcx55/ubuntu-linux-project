#include<iostream>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>

#define PAGE_SIZE 4096
//write mmap
int main(int argc,char** argv){
    //mmap要求自己先打开文件！！
    if(argc!=2){
        std::cout<<"exe filename"<<std::endl;
        exit(0);
    }
    int fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd<0){
        std::cout<<"失败"<<std::endl;
        return 0;
    }
    //TODO
    //2、手动调整文件大小！
    if(ftruncate(fd,PAGE_SIZE)==-1){
        std::cout<<"ftruncate函数失败"<<std::endl;
        return 0;
    }

    //3\mmap
    char* shmaddr=(char*)mmap(nullptr,PAGE_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(shmaddr==MAP_FAILED){
        std::cout<<"mmap失败"<<std::endl;
        return 0;        
    }

    //4、文件编写！ 使用shmaddr即可
    for(int i=0;i<4096;++i){
        *(shmaddr+i)=i%26+'a';
    }


    //5、取消映射
    if(munmap(shmaddr,PAGE_SIZE)==-1){
        std::cout<<"映射失败"<<std::endl;
        return 0;
    }

    close(fd);
    return 0;
}