//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_RESERVATIONSTATION_LOADSTOREBUFFER_H
#define CODE_RESERVATIONSTATION_LOADSTOREBUFFER_H
#include <iostream>
#include "bus.h"
#include "registerfile.h"
#include "reorderbuffer.h"
#include "alu.h"
const int RSsize=16;

class ReservationStation{
private:
    RSunit rs_next[RSsize];
public:
    RSunit rs[RSsize];
public:
    void refresh(){
        for(int i=0;i<RSsize;i++){
            rs[i]=rs_next[i];
        }
    }

    void flush(){//如果分支错误，则把RS清空
        for(int i=0;i<RSsize;i++){
            rs_next[i].busy=false;
        }
    }

    bool available(){
        for(int i=0;i<RSsize;i++){
            if(!rs[i].busy){
                return true;
            }
        }
        return false;
    }

    void add(InstructionUnit ins,int number,RegisterFile *RF){
        int index=-1;
        for(int i=0;i<RSsize;i++){
            if(!rs[i].busy){
                index=i;
                break;
            }
        }
        if(index==-1){
            std::cout<<"RS is full\n";
            return;
        }else{
            RSunit tmp;
            tmp.busy=true;
            tmp.op=ins.ins;
            tmp.RoBindex=number;
            int num= rs_cnt(tmp.op);
            if(num==2){
                if(!RF->regs[ins.rs1].busy){//如果没有依赖，直接读取数值
                    tmp.vj=RF->regs[ins.rs1].data;
                    tmp.qj=-1;
                }else{//否则，将依赖的编号存入qj
                    tmp.qj=RF->regs[ins.rs1].rely;
                }

                if(!RF->regs[ins.rs2].busy){//如果没有依赖，直接读取数值
                    tmp.vk=RF->regs[ins.rs2].data;
                    tmp.qk=-1;
                }else{//否则，将依赖的编号存入qk
                    tmp.qk=RF->regs[ins.rs2].rely;
                }
            }else if(num==1){
                if(!RF->regs[ins.rs1].busy){//如果没有依赖，直接读取数值
                    tmp.vj=RF->regs[ins.rs1].data;
                    tmp.qj=-1;
                }else{//否则，将依赖的编号存入qj
                    tmp.qj=RF->regs[ins.rs1].rely;
                }

                tmp.vk=ins.imm;
                tmp.qk=-1;
            }
            rs_next[index]=tmp;
        }
    }

    void execute(ALU *alu,ReorderBuffer *RoB){//选出一条可以执行的指令，将计算结果交给RoB，并通知更新所有RS
        for(int i=0;i<RSsize;i++){
            if(rs[i].busy){//有指令
                if(rs[i].qj==-1&&rs[i].qk==-1){//可以执行
                    int result=alu->execute(rs[i].op,rs[i].vj,rs[i].vk);//通过alu运算
                    rs_next[i].busy= false;
                    RoB->finish_calc(rs[i].RoBindex,result);
                    CDB_value cdb;
                    cdb.RoB_index=rs[i].RoBindex;
                    cdb.value=result;
                    broadcast(cdb);
                    break;
                }
            }
        }
    }

    void broadcast(CDB_value res){//模拟计算出的结果通过CDB向RS广播
        for(int i=0;i<RSsize;i++){
            if(!rs[i].busy){
                continue;
            }else{
                if(rs[i].qj==res.RoB_index){//如果qj的依赖正好是广播的指令，则更新依赖
                    rs_next[i].qj=-1;
                    rs_next[i].vj=res.value;
                }
                if(rs[i].qk==res.RoB_index){
                    rs_next[i].qk=-1;
                    rs_next[i].vk=res.value;
                }
            }
        }
    }
};


struct LSBentry{
    Instr op;
    int RoBindex;
    int vj,vk;
    int qj=-1,qk=-1;
    unsigned int imm;
    int A;//Load或Store的地址
};

class LoadStoreBuffer{
private:
    Queue<LSBentry,16>LSB_list_next;
public:
    Queue<LSBentry,16>LSB_list;

public:
    void refresh(){
        LSB_list=LSB_list_next;
    }

    void flush(){//清空LSB
        LSB_list_next.clear();
    }

    bool available(){
        if(LSB_list.full()){
            return false;
        }else{
            return true;
        }
    }

    void add(InstructionUnit Ins,int index,RegisterFile *RF){
        if(LSB_list.full()){
            std::cout<<"LSB is full\n";
            return;
        }else{
            LSBentry tmp;
            tmp.op=Ins.ins;
            tmp.RoBindex=index;
            switch (tmp.op) {
                case Lb:
                case Lh:
                case Lw:
                case Lbu:
                case Lhu:{
                    if(!RF->regs[Ins.rs1].busy){//如果没有依赖，直接读取数值
                        tmp.vj=RF->regs[Ins.rs1].data;
                        tmp.qj=-1;
                    }else{//否则，将依赖的编号存入qj
                        tmp.qj=RF->regs[Ins.rs1].rely;
                    }
                    tmp.imm=Ins.imm;
                    break;
                }
//                case Sb:
//                case Sh:
//                case Sw:{
//                    if(!RF->regs[Ins.rs1].busy){//如果没有依赖，直接读取数值
//                        tmp.vj=RF->regs[Ins.rs1].data;
//                        tmp.qj=-1;
//                    }else{//否则，将依赖的编号存入qj
//                        tmp.qj=RF->regs[Ins.rs1].rely;
//                    }
//
//                    if(!RF->regs[Ins.rs2].busy){//如果没有依赖，直接读取数值
//                        tmp.vk=RF->regs[Ins.rs2].data;
//                        tmp.qk=-1;
//                    }else{//否则，将依赖的编号存入qk
//                        tmp.qk=RF->regs[Ins.rs2].rely;
//                    }
//
//                    tmp.imm=Ins.imm;
//                    break;
//                }
                default:
                    break;
            }
            LSB_list_next.push(tmp);
        }
    }

    void execute(Memory *mem){
        if(!LSB_list.empty()){
            LSBentry obj=LSB_list.top();
            switch (obj.op) {
                case Lb:
                case Lh:
                case Lw:
                case Lbu:
                case Lhu:{
                    if(obj.qj==-1){
                        int addr=obj.vj+obj.imm;
                        int result=mem->load(addr,obj.op);
                        LSB_list_next.pop();
                    }
                    break;
                }
//               case Sb:
//               case Sh:
//               case Sw:{
//                   if(obj.qj==-1&&obj.qk==-1){
//                       int addr=obj.vj+obj.imm;
//
//                   }
//                   break;
//               }
                default:
                    break;
            }
        }
    }
};

#endif //CODE_RESERVATIONSTATION_LOADSTOREBUFFER_H
