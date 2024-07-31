//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_RESERVATIONSTATION_H
#define CODE_RESERVATIONSTATION_H
#include "bus.h"
#include "registerfile.h"
const int RSsize=16;

struct RSunit{
    int RoBindex;//在RoB中的编号
    bool busy=false;
    Instr op;
    int vj,vk;
    int qj,qk;
};

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

    void execute(){
        for(int i=0;i<RSsize;i++){
            if(rs[i].busy){//有指令
                if(rs[i].qj==-1&&rs[i].qk==-1){//可以执行
                    if(rs[i].op==Jal){

                    }else if(rs[i].op==Jalr){

                    }else if(rs[i].op==Lui){

                    }else if(rs[i].op==Auipc){

                    }else{

                    }
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

#endif //CODE_RESERVATIONSTATION_H
