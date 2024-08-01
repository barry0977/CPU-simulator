//
// Created by 24115 on 2024/7/26.
//

#ifndef CODE_MEMORY_H
#define CODE_MEMORY_H
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "decode.h"
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
    unsigned int memory[4096*100]={};
public:
    void read(){
        std::string line;
        unsigned int place;//在内存中写的位置
        while(std::getline(std::cin,line)){
            if(line.empty()){
                continue;
            }
            else if(line[0]=='@'){
                std::stringstream ss;
                ss<<std::hex<<line.substr(1,8);
                ss>>place;
            }
            else{
                std::istringstream iss(line);
                std::string token;
                unsigned int cnt=0;
                unsigned int value=0;
                while(iss>>token){
                    cnt++;
                    value+= TokenToInt(token)<<8*(cnt-1);
                    if(cnt==4){
                        write_word(value,place);
//                        std::cout<< std::hex << std::setfill('0') << std::setw(8)<<value<<std::endl;
                        place+=4;
                        cnt=value=0;
                    }
                }
            }
        }
    }

    void write_word(unsigned int &data,int index){
//        std::cout<<"地址： "<<index<<"  "<<std::hex << std::setfill('0') << std::setw(8)<<data<<std::endl;
        memory[index]=data;
    }

    unsigned int read_word(int index){
        return memory[index];
    }

    int load_byte(int index){//读出一个字节
        int addr=index-index%4;
        int order=index%4;
        int data=memory[addr];
        data=data>>(8*order)&0b11111111;
        int res=static_cast<int>(data<<24)>>24;//符号扩展
        return res;
    }

    int load_half(int index){//读出两个字节
        int addr=index-index%4;
        int order=index%4;
        int data=memory[addr];
        data=data>>(8*order)&0xFF;
        int res=static_cast<int>(data<<16)>>16;//符号扩展
        return res;
    }

    int load_word(int index){//读出四个字节
        return memory[index];
    }

    int load_byte_u(int index){
        int addr=index-index%4;
        int order=index%4;
        int data=memory[addr];
        data=data>>(8*order)&0b11111111;//无需符号扩展
        return data;
    }

    int load_half_u(int index){
        int addr=index-index%4;
        int order=index%4;
        int data=memory[addr];
        data=data>>(8*order)&0xFF;//无需符号扩展
        return data;
    }

    int load(int index,Instr op){
        switch(op){
            case Lb:return load_byte(index);
            case Lh:return load_half(index);
            case Lw:return load_word(index);
            case Lbu:return load_byte_u(index);
            case Lhu:return load_half_u(index);
        }
    }

    void store_byte(int data,int index){//写入一个字节
        data=data&0xFF;
        int addr=index-index%4;
        int order=index%4;
        if(order==0){
            memory[addr]&=0xFFFFFF00;//把第一个字节置为0
            memory[addr]|=data;
        }else if(order==1){
            memory[addr]&=0xFFFF00FF;//把第二个字节置为0
            memory[addr]|=(data<<8);
        }else if(order==2){
            memory[addr]&=0xFF00FFFF;//把第三个字节置为0
            memory[addr]|=(data<<16);
        }else{
            memory[addr]&=0x00FFFFFF;//把第四个字节置为0
            memory[addr]|=(data<<24);
        }
    }

    void store_half(int data,int index){//写入两个字节
        data=data&0xFFFF;
        int addr=index-index%4;
        int order=index%4;
        if(order==0){
            memory[addr]&=0xFFFF0000;//把前两个字节置为0
            memory[addr]|=data;
        }else{
            memory[addr]&=0x0000FFFF;//把后两个字节置为0
            memory[addr]|=(data<<16);
        }
    }

    void store_word(int data,int index){//写入四个字节
        memory[index]=data;
    }

    void store(int data,int index,Instr op){
        switch(op){
            case Sb:
                store_byte(data,index);
                break;
            case Sh:
                store_half(data,index);
                break;
            case Sw:
                store_word(data,index);
                break;
        }
    }
};
#endif //CODE_MEMORY_H
