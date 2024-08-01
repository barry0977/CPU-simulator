//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_CPU_H
#define CODE_CPU_H
#include "reorderbuffer.h"
#include "reservationstation_loadstorebuffer.h"
#include "registerfile.h"
#include "loadstorebuffer.h"
#include "instructionqueue.h"
#include "alu.h"


class CPU{
private:
    Memory *mem;
    ReorderBuffer RoB;
    ReservationStation RS;
    RegisterFile RF;
    LoadStoreBuffer LSB;
    InstructionQueue IQ;
    ALU alu;

public:
    CPU(Memory *memory){
        mem=memory;
    }

    void init(){
        IQ.init(mem);

    }

    void run(){
        int clk=0;
        while(true){
            clk++;
            execute();
            refresh();
        }
    }

    void refresh(){
        RoB.refresh();
        RS.refresh();
        RF.refresh();
        LSB.refresh();
        IQ.refresh();
    }

    void flush(unsigned int addr){//清空RoB,RS和LSB，重置IQ的PC
        RoB.flush();
        RS.flush();
        LSB.flush();
        RF.flush();
        IQ.set_PC(addr);
    }

    void execute(){
        int res=RoB.commit(&RF,mem,&LSB);
        RS.execute(&alu,&RoB);
        LSB.execute(mem);
        if(res>=0){//如果发现预测错误
            flush(res);
        }
    }
};

#endif //CODE_CPU_H
