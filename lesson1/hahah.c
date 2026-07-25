#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>
int main(){
    int pipeint[2]={0};
    int n=pipe(pipeint);
    if(n<0){
        perror("pipe:");
        return 1;
    }
    else{
        printf("the pipe is success!,pipt[0]=%d,pipe[1]=%d\n",pipeint[0],pipeint[1]);
    }
    pid_t id=fork();
    char msg[4096]={"hello vscode!\0"};
    if(id==0){
        //child  w
        char outbuffer[1024];//操了 写入还有写时拷贝！
        close(pipeint[0]);
        // write(pipeint[1],msg,strlen(msg));
        int cnt=10;
        while(cnt){
        snprintf(outbuffer,sizeof(outbuffer),"hello vscode第%d次,子进程pid= %d\n",cnt,getpid());
        write(pipeint[1],outbuffer,strlen(outbuffer));//
        sleep(10);
        --cnt;
        }
        printf("我是子进程 %s\n",outbuffer);
        exit(0);
    }
    int status=0;
   pid_t rid=waitpid(id,&status,0);
   if(rid==0){
    perror("wait failed!\n");
   }
//    printf("等待成功!,等待的子进程pid=%d\n",rid);
    close(pipeint[1]);
    char gg[9000]={'\0'};
    read(pipeint[0],gg,1024);
    printf("%s\n",gg);
    return 0;
}