#pragma once
#include <iostream>
#include "/usr/include/jsoncpp/json/json.h"
#include "logger.hpp"
#include <functional>
// 请求
class request
{
public:
    request(int x = 0, int y = 0, char op = 0)
        : _data_x(x), _data_y(y), _oper(op)
    {
    }
    ~request()
    {
    }
    bool Serialize(std::string &a)
    { // 序列化  转为字符串
        Json::Value value;
        value["x"] = _data_x;
        value["y"] = _data_y;
        value["op"] = _oper;
        Json::FastWriter write;
        a = write.write(value);
        return true;
    }
    bool DeSerialize(std::string &b)
    {
        Json::Value value;
        Json::Reader read;
        bool pa = read.parse(b, value); // 远端发来一个JSON看得懂的字符串，将这个转换为value！
        if (!pa)
        {
            std::cout << "parse 反序列化失败！" << std::endl;
            return false;
        }
        _data_x = value["x"].asInt();
        _data_y = value["y"].asInt();
        _oper = value["op"].asInt();
        return true;
    }

    // private:
public:
    int _data_x;
    int _data_y;
    char _oper;
};

// 回复
class response
{
public:
    response(int result = 0, int code = 0)
        : _result(result), _code(code)
    {
    }
    ~response()
    {
    }
    bool DeSerialize(std::string &b) // 反序列化！
    {
        Json::Value value;
        Json::Reader read;
        bool pa = read.parse(b, value); // 远端发来一个JSON看得懂的字符串，将这个转换为value！
        if (!pa)
        {
            std::cout << "parse 反序列化失败！" << std::endl;
            return false;
        }
        _result = value["result"].asInt();
        _code = value["code"].asInt();
        // _oper = value["op"].asInt();
        return true;
    }
    bool Serialize(std::string &a)
    { // 序列化  转为字符串
        Json::Value value;
        value["result"] = _result;
        value["code"] = _code;
        Json::FastWriter write;
        a = write.write(value);
        return true;
    }

    // private:
public:
    int _result;
    int _code; // 状态码！
};
using cr=void(*)(response&);

static const std::string stp = "\r\n";
using handler_t = std::function<response(request &)>;

class protocol
{
public:
    protocol(handler_t g)
        : version("1.0"), _cb(g)
    {
    }
    protocol(cr g)
        : version("1.0"), _cr(g)
    {
    }

    std::string Pack(const std::string &Jsonstring)
    {
        return std::to_string(Jsonstring.size()) + stp + Jsonstring + stp;
    }
    int Unpack(std::string &packet, std::string *Jsonstring)
    {
        if (packet.size() == 0)
        {
            return 0;
        }
        if (Jsonstring == nullptr)
        {
            return -1;
        }
        // 分析报文
        auto pos = packet.find(stp);
        if (pos == std::string::npos)
        { // 要是pos都没有 就是长度没有发全！ 第一个\r\n都没
            return 0;
        }
        std::string pstr = packet.substr(0, pos);
        int len = std::stoi(pstr.c_str());
        int totalbaowenlen = len + pstr.size() + 2 * stp.size();
        if (packet.size() < totalbaowenlen)
        {
            return 0;
        }
        *Jsonstring = packet.substr(pos + stp.size(), len);
        packet.erase(0, totalbaowenlen);//解包 得到Json串后，删除全部完整的报文！
        return 1;
    }

    std::string requestParse(std::string &inbuff)
    {
        //传来的长度
        std::string allresult;//为什么要在外面？因为有些解析失败了 要保留序列化 以及size！
        while (true)
        {//只要这里有一个break 说明报文这里是不完整的！！
            std::string Jsonstring;
            int n = Unpack(inbuff, &Jsonstring);
            if (n < 0)
            {
                log << "[error] Parse Unpack error,such as:Jsonstring is nullptr" << '\n';
                // return "";
                break;
            }
            else if (n == 0)
            {
                log << "[info] recv is not all baowen" << '\n';
                // return "";
                break;
            }
            // 牺牲这么多，得到完整的Jsonstring的string 需要JSON翻译，reader的parse回去
            request pq;//
            if (!pq.DeSerialize(Jsonstring))
            {
                log << "[error] server Deserialize error" << '\n';
                // return "";
                break;
            }
            response res;
            if (_cb)
                res = _cb(pq); // 计算完毕，进行序列化
            std::string Get;//输出型参数
            if (!res.Serialize(Get))
            {
                log << "[error] server serialize error" << '\n';
                // return "";
                break;
            }
            allresult += Pack(Get);
        }
        return allresult;
    }
    std::string reponseParse(std::string &inbuff)
    {
        //传来的长度
        // std::string allresult;//为什么要在外面？因为有些解析失败了 要保留序列化 以及size！
        // while (true)
        {
            std::string Jsonstring;
            int n = Unpack(inbuff, &Jsonstring);
            if (n < 0)
            {
                log << "[error] Parse Unpack error,such as:Jsonstring is nullptr" << '\n';
                // return "";
                return "";
            }
            else if (n == 0)
            {
                log << "[info] recv is not all baowen" << '\n';
                return "";
                // break;
            }
            // 牺牲这么多，得到完整的Jsonstring的string 需要JSON翻译，reader的parse回去
            response pq;//
            if (!pq.DeSerialize(Jsonstring))
            {
                log << "[error] server Deserialize error" << '\n';
                return "";
                // break;
            }
            if(_cr)
            _cr(pq);

            // response res;
            // // if (_cb)
            // //     res = _cb(pq); // 计算完毕，进行序列化
            // std::string Get;//输出型参数
            // if (!res.Serialize(Get))
            // {
            //     log << "[error] server serialize error" << '\n';
            //     // return "";
            //     break;
            // }
            // allresult += Pack(Get);
        }
        return "yes";
        // return allresult;
    }

private:
    std::string version;
    handler_t _cb;
    cr _cr;
};