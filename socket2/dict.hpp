#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
static std::string str = "./dict.txt";
class dict
{
public:
    dict()
    {
        Loaddict();
    }
    void Loaddict()
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            std::cerr << "字典加载失败，功能异常！" << std::endl;
            exit(1);
        }
        std::string cap;
        static std::string ctr = ": ";
        while (getline(in, cap))
        {
            size_t n = cap.find(ctr);
            std::string k = cap.substr(0, n);
            std::string v = cap.substr(n + ctr.size());
            tree.insert(std::make_pair(k, v));
        }
        in.close();
    }
    std::string translate(std::string &name)
    {
        auto iter = tree.find(name);
        if (iter != tree.end())
        {
            return tree[name];
        }
        else
        {
            return "None";
        }
    }

private:
    std::string path = str;
    std::unordered_map<std::string, std::string> tree;
};