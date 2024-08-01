//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_RESERVATIONSTATION_H
#define CODE_RESERVATIONSTATION_H
#include <iostream>
#include "bus.h"
#include "registerfile.h"
#include "reorderbuffer.h"
#include "alu.h"
#include "cdb.h"
const int RSsize=32;

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

    void add_jalr(InstructionUnit ins,int number,RegisterFile *RF,CDB *cdb){
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
            if(!RF->regs[ins.rs1].busy){
                tmp.vj=RF->regs[ins.rs1].data;
                tmp.qj=-1;
            }else{
                tmp.qj=RF->regs[ins.rs1].rely;
                if(cdb->num==1){
                    if(cdb->update.RoB_index==tmp.qj){
                        tmp.vj=cdb->update.value;
                        tmp.qj=-1;
                    }
                }
            }
            tmp.vk=ins.imm;
            tmp.qk=-1;
            tmp.RoBindex=number;
            tmp.result=ins.PC+4;
            tmp.op=ins.ins;
            rs_next[index]=tmp;
        }
    }

    void add(InstructionUnit ins,int number,RegisterFile *RF,CDB *cdb){
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
                    if(cdb->num==1){
                        if(cdb->update.RoB_index==tmp.qj){
                            tmp.vj=cdb->update.value;
                            tmp.qj=-1;
                        }
                    }
                }

                if(!RF->regs[ins.rs2].busy){//如果没有依赖，直接读取数值
                    tmp.vk=RF->regs[ins.rs2].data;
                    tmp.qk=-1;
                }else{//否则，将依赖的编号存入qk
                    tmp.qk=RF->regs[ins.rs2].rely;
                    if(cdb->num==1){
                        if(cdb->update.RoB_index==tmp.qk){
                            tmp.vk=cdb->update.value;
                            tmp.qk=-1;
                        }
                    }
                }
            }else if(num==1){
                if(!RF->regs[ins.rs1].busy){//如果没有依赖，直接读取数值
                    tmp.vj=RF->regs[ins.rs1].data;
                    tmp.qj=-1;
                }else{//否则，将依赖的编号存入qj
                    tmp.qj=RF->regs[ins.rs1].rely;
                    if(cdb->num==1){
                        if(cdb->update.RoB_index==tmp.qj){
                            tmp.vj=cdb->update.value;
                            tmp.qj=-1;
                        }
                    }
                }

                tmp.vk=ins.imm;
                tmp.qk=-1;
            }else if(num==0){
                tmp.qj=tmp.qk=-1;
                tmp.vj=ins.imm;
                tmp.vk=ins.PC;
            }
            rs_next[index]=tmp;
        }
    }

    void show(){
        std::cout<<"----------- RS ---------------------\n";
        for(int i=0;i<RSsize;i++){
            if(rs[i].busy){
                std::cout<<"rs["<<i<<"] is busy: "<<rs[i].op<<" index in RoB:"<<rs[i].RoBindex<<" vj:"<<rs[i].vj<<" qj:"<<rs[i].qj<<" vk:"<<rs[i].vk<<" qk:"<<rs[i].qk<<" "<<std::endl;
            }
        }
    }

    void execute(ALU *alu,ReorderBuffer *RoB,CDB *cdb){//选出一条可以执行的指令，将计算结果交给RoB，并通知更新所有RS
        if(cdb->num==1){
            CDB_value newinf=cdb->update;
            broadcast(newinf);
        }
        for(int i=0;i<RSsize;i++){
            if(rs[i].busy){//有指令
                if(rs[i].qj==-1&&rs[i].qk==-1){//可以执行
                    if(rs[i].op==Jalr){//对Jalr特殊处理
                        int PCaddr=rs[i].vj+rs[i].vk;
                        RoB->finish_jalr(rs[i].RoBindex,PCaddr);//传入要去的地址
                        RoB->finish_calc(rs[i].RoBindex,rs[i].result);//传入修改的寄存器的值
                        rs_next[i].busy=false;
//                        CDB_value obj;
//                        obj.RoB_index=rs[i].RoBindex;
//                        obj.value=rs[i].result;
//                        broadcast(obj);
//                        cdb->send_value(obj,toLSB);
                    }else{
                        int result=alu->execute(rs[i].op,rs[i].vj,rs[i].vk);//通过alu运算
                        rs_next[i].busy= false;
                        RoB->finish_calc(rs[i].RoBindex,result);
//                        CDB_value obj;
//                        obj.RoB_index=rs[i].RoBindex;
//                        obj.value=result;
//                        broadcast(obj);
//                        cdb->send_value(obj,toLSB);
                    }
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

//    void get_from_LSB(CDB *cdb){
//        package get=cdb->get_value(toRS);
//        if(get.has){
//            broadcast(get.data);
//        }
//    }
};

#endif //CODE_RESERVATIONSTATION_H
