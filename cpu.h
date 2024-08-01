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
            std::cout<<"clk: "<<clk<<std::endl;
            clk++;
            execute();
            RF.show();
            RoB.show();
            RS.show();
            LSB.show();
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
        IQ.execute(&RoB,&RS,&LSB,&RF);
        int res=RoB.commit(&RF,mem);
        RS.execute(&alu,&RoB,&cdb);
        LSB.execute(mem,&RoB,&cdb);
        RS.get_from_LSB(&cdb);//从LSB接受信息更新
        LSB.get_from_RS(&cdb);//从RS接受信息更新
        if(res>=0){//如果发现预测错误
            flush(res);
        }
    }
};

#endif //CODE_CPU_H
