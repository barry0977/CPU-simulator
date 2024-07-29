#include <iostream>
#include <bitset>
#include "memory.h"
int main(){
    int ins=2143286541;
    int imm=((ins>>31)<<20)|(((ins>>21)&0b1111111111)<<1)|(((ins>>20)&1)<<11)|(((ins>>12)&0b11111111)<<12);
    std::cout<<std::bitset<32>(imm)<<std::endl;
    imm = static_cast<int>(imm << 11) >> 11;
    std::cout<<std::bitset<32>(imm)<<std::endl;
//    freopen("in.txt","r",stdin);
//    Memory memory;
//    memory.read();
}