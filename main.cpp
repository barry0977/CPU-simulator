#include <iostream>
#include <bitset>
#include "memory.h"
#include "cpu.h"
int main(){
    freopen("in.txt","r",stdin);
    Memory memory;
    memory.read();
    CPU cpu(&memory);
    cpu.init();
    cpu.run();
}