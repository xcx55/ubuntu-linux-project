#include "Tcpserver.hpp"
#include "protocol.hpp" //协议
#include <memory>
#include "cal.hpp"
using namespace _SOCKET;
int main(int argc, char *argv[])
{
    //1、计算机层
    std::unique_ptr<cal> calculator = std::make_unique<cal>();
    //2、协议层
    std::unique_ptr<protocol> pt = std::make_unique<protocol>([&calculator](request& a) ->
    response{
        return calculator->calc(a);
    });
    if (argc != 2)
    {
        log << "[error] 没有写端口" << '\n';
    }
    //3、服务层
    std::unique_ptr<Tcpserver> server = std::make_unique<Tcpserver>([&pt](std::string &inbuff)
                                                                        -> std::string
                                                                    { return pt->requestParse(inbuff); },
                                                                    argv[1]);
    server->Loop();
    return 0;
}