//
// Created by 24115 on 2024/7/29.
//

#ifndef CODE_LOADSTOREBUFFER_H
#define CODE_LOADSTOREBUFFER_H
#include "myqueue.h"
//#include "registerfile.h"

struct LSBentry{
    Instr op;
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

    void add(InstructionUnit Ins,RegisterFile *RF){
        if(LSB_list.full()){
            std::cout<<"LSB is full\n";
            return;
        }else{
            LSBentry tmp;
            tmp.op=Ins.ins;
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
                case Sb:
                case Sh:
                case Sw:{
                    if(!RF->regs[Ins.rs1].busy){//如果没有依赖，直接读取数值
                        tmp.vj=RF->regs[Ins.rs1].data;
                        tmp.qj=-1;
                    }else{//否则，将依赖的编号存入qj
                        tmp.qj=RF->regs[Ins.rs1].rely;
                    }

                    if(!RF->regs[Ins.rs2].busy){//如果没有依赖，直接读取数值
                        tmp.vk=RF->regs[Ins.rs2].data;
                        tmp.qk=-1;
                    }else{//否则，将依赖的编号存入qk
                        tmp.qk=RF->regs[Ins.rs2].rely;
                    }

                    tmp.imm=Ins.imm;
                    break;
                }
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

                   }
               }
               case Sb:
               case Sh:
               case Sw:{
                   if(obj.qj==-1&&obj.qk==-1){

                   }
               }
               default:
                   break;
           }
       }
    }
};
#endif //CODE_LOADSTOREBUFFER_H
