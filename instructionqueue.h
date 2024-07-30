//
// Created by 24115 on 2024/7/30.
//

#ifndef CODE_INSTRUCTIONQUEUE_H
#define CODE_INSTRUCTIONQUEUE_H
#include "memory.h"
#include "instructions.h"
#include "decode.h"
#include "myqueue.h"
class InstructionQueue{
private:
    Queue<InstructionUnit,32>IQ_next;
    int PC_next;
    Memory *mem;
public:
    Queue<InstructionUnit,32>IQ;
    int PC;

public:
    void init(Memory *memory){
        PC=0;
        PC_next=4;
        mem=memory;
    }

    void refresh(){
        PC=PC_next;
        PC_next=PC+4;
        IQ=IQ_next;
    }

    void IF(){
        unsigned int itr;
        mem->read_word(itr,PC);
        InstructionUnit Ins= decode(itr);
        IQ_next.push(Ins);
    }
};

bool predictor(){

}

#endif //CODE_INSTRUCTIONQUEUE_H
