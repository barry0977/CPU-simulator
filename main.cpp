#pragma GCC optimize(3,"Ofast","inline")
#include <iostream>
#include <bitset>
#include "memory.h"
#include "cpu.h"
int main(){
//    freopen("in.txt","r",stdin);
//    freopen("out2.txt","w",stdout);
    Memory memory;
    memory.read();
    CPU cpu(&memory);
    cpu.init();
    cpu.run();
}