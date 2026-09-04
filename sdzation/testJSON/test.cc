#include<iostream>
#include "/usr/include/jsoncpp/json/json.h"
#include<string>
int main(){
    Json::Value value;
    value["sex"]="男";
    Json::StyledWriter writer;
    std::string p=writer.write(value);
    std::cout<<p<<std::endl;
    return 0;
}