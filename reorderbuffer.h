//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REORDERBUFFER_H
#define CODE_REORDERBUFFER_H
#include "myqueue.h"
#include "instructions.h"
class ReorderBuffer{
private:
    Queue<InstructionUnit,20>list;

public:
    void launch(){

    }

    void commit(){

    }
};

#endif //CODE_REORDERBUFFER_H
