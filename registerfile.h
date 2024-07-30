//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REGISTERFILE_H
#define CODE_REGISTERFILE_H
#include "register.h"
class RegisterFile{
private:
    Register regs_next[32];
public:
    Register regs[32];

public:
    void refresh(){
        for(int i=0;i<32;i++){
            regs[i]=regs_next[i];
            regs_next[i]=regs[i];
        }
    }
};

#endif //CODE_REGISTERFILE_H
