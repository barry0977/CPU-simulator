//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_CPU_H
#define CODE_CPU_H
#include "reorderbuffer.h"
#include "reservationstation.h"
#include "loadstorebuffer.h"
#include "registerfile.h"
#include "instructionqueue.h"
#include "alu.h"
#include "cdb.h"
#include "predictor.h"

class CPU{
private:
    Memory *mem;
    ReorderBuffer RoB;
    ReservationStation RS;
    RegisterFile RF;
    LoadStoreBuffer LSB;
    InstructionQueue IQ;
    ALU alu;
    CDB cdb;
    Predictor pre;

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
//            std::cout<<"clk: "<<clk<<std::endl;
            clk++;
            execute();
//            RF.show();
//            RoB.show();
//            RS.show();
//            LSB.show();
            refresh();
        }
    }

    void refresh(){
        RoB.refresh();
        RS.refresh();
        RF.refresh();
        LSB.refresh();
        IQ.refresh();
        cdb.refresh();
    }

    void flush(unsigned int addr){//清空RoB,RS和LSB，重置寄存器状态，重置IQ的PC
        //还要清空IQ!
        RoB.flush();
        RS.flush();
        LSB.flush();
        RF.flush();
        IQ.flush();
        IQ.set_PC(addr);
    }

    void execute(){
        LSB.execute(mem,&RoB,&cdb);
        RS.execute(&alu,&RoB,&cdb);
        int res=RoB.commit(&RF,mem,&cdb,&pre);
        IQ.execute(&RoB,&RS,&LSB,&RF,&cdb,&pre);
//        IQ.execute(&RoB,&RS,&LSB,&RF,&cdb,&pre);
//        int res=RoB.commit(&RF,mem,&cdb,&pre);
//        RS.execute(&alu,&RoB,&cdb);
//        LSB.execute(mem,&RoB,&cdb);
        if(res>=0){//如果发现预测错误
            flush(res);
        }
    }
};

#endif //CODE_CPU_H
