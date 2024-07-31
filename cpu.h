//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_CPU_H
#define CODE_CPU_H
#include "reorderbuffer.h"
#include "reservationstation.h"
#include "registerfile.h"
#include "loadstorebuffer.h"
#include "instructionqueue.h"
#include "alu.h"


class CPU{
private:
    Memory *mem;
    ReorderBuffer RB;
    ReservationStation RS;
    RegisterFile RF;
    LoadStoreBuffer LSB;
    InstructionQueue IQ;
    ALU alu;

public:
    CPU(Memory *memory){
        mem=memory;
    }

    void run(){

    }

    void refresh(){
        RB.refresh();
        RS.refresh();
        RF.refresh();
        LSB.refresh();
    }

    void execute(){
        RB.execute();
        RS.execute();
        R
    }
};

#endif //CODE_CPU_H