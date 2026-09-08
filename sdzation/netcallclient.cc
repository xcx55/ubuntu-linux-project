#include<iostream>
#include"protocol.hpp"
#include"socket.hpp"
using namespace _SOCKET;
void Print(response& x){
    std::cout<<x._result<<std::endl;
}
int main(int argc,char* argv[]){
    std::unique_ptr<Socket> client=std::make_unique<TcpSocket>(argv[1],argv[2]);
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    client->CreatSocketOrDie();
    client->Connect();
    //干事情
        std::string Getbaowen;

    while(true){
        //0、准备数据
        int x,y;
        char op;
        std::cout<<"please enter x y"<<std::endl;
        std::cin>>x;
        std::cin>>y;
        std::cout<<"please enter your op"<<std::endl;
        std::cin>>op;
        //1、数据运算！
        request p(x,y,op);
        std::string tofuwu;
        //序列化请求
        p.Serialize(tofuwu);
        protocol pa(nullptr);
        //封包请求
        std::string Getpackedstring=pa.Pack(tofuwu);
        //发送
        ssize_t n=client->Send(Getpackedstring);//send的时候不会阻塞 只会切换内核态！
        client->Recv(&Getbaowen);//没有就阻塞
        //TCP收集到的 inbuff  不是一段一段的
        protocol a(Print);
        log<<"[info] 接收客户端收到服务端信息,服务端的信息不一定是完整的,客户端开始解析"<<'\n';
        std::string pq=a.reponseParse(Getbaowen);
        if(pq==""){
            continue;
        }
        Getbaowen.clear();

    }

    client->Close();
    return 0;
}