//
// Created by 24115 on 2024/7/26.
//

#ifndef CODE_MEMORY_H
#define CODE_MEMORY_H
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
unsigned int TokenToInt(std::string str){
    unsigned int ans=0;
    if(str[1]>='0'&&str[1]<='9'){
        ans+=str[1]-'0';
    }else{
        ans+=str[1]-'A'+10;
    }
    if(str[0]>='0'&&str[0]<='9'){
        ans+=(str[0]-'0')<<4;
    }else{
        ans+=(str[0]-'A'+10)<<4;
    }
    return ans;
}

class Memory{
private:
    unsigned int memory[4096]={};
public:
    void read(){
        std::string line;
        unsigned int place;//在内存中写的位置
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
                unsigned int value=0;
                while(iss>>token){
                    cnt++;
                    value+= TokenToInt(token)<<8*(cnt-1);
                    if(cnt==4){
                        write_word(value,place);
                        std::cout<< std::hex << std::setfill('0') << std::setw(8)<<value<<std::endl;
                        place+=4;
                        cnt=value=0;
                    }
                }
            }
        }
    }

    void write_word(unsigned int &data,int index){
        memory[index]=data;
    }

    void read_word(unsigned int &data,int index){
        data=memory[index];
    }
};

#endif //CODE_MEMORY_H
