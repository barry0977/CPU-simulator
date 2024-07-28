//
// Created by 24115 on 2024/7/26.
//

#ifndef CODE_MEMORY_H
#define CODE_MEMORY_H
#include <iostream>
#include <string>
#include <sstream>

int TokenToInt(std::string str){
    int ans=0;
    if(str[1]>='0'&&str[1]<='9'){
        ans+=str[1]-'0';
    }else{
        ans+=str[1]-'A'+10;
    }
    if(str[0]>='0'&&str[0]<='9'){
        ans+=(str[1]-'0')*;
    }else{
        ans+=str[1]-'A'+10;
    }
}

class Memory{
private:
    int memory[4096]={};
public:
    void read(){
        std::string line;
        int place;//在内存中写的位置
        while(std::getline(std::cin,line)){
            if(line.empty()){
                continue;
            }
            else if(line[0]=='@'){
                place=std::stoi(line.substr(1,8));
            }
            else{
                std::istringstream iss(line);
                std::string token;
                int cnt=0;
                int value=0;
                while(iss>>token){
                    cnt++;
                    value+=
                }
            }
        }
    }
};

#endif //CODE_MEMORY_H
