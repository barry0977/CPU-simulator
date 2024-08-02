//
// Created by 24115 on 2024/8/1.
//

#ifndef CODE_LOADSTOREBUFFER_H
#define CODE_LOADSTOREBUFFER_H
#include "bus.h"
#include "myqueue.h"
#include "memory.h"
#include "registerfile.h"
#include "reorderbuffer.h"
#include "cdb.h"

const int LSBsize=32;
class LoadStoreBuffer{
private:
    Queue<LSBentry,LSBsize>LSB_list_next;
public:
    Queue<LSBentry,LSBsize>LSB_list;

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

    void show(){
        std::cout<<"----------- LSB ---------------------\n";
        if(LSB_list.empty()){
            std::cout<<"LSB is empty\n";
        }else{
            int beg=LSB_list.front;
            int end=LSB_list.rear;
            if(end>beg){
                for(int i=beg+1;i<=end;i++){
                    std::cout<<LSB_list[i].op<<" index in RoB:"<<LSB_list[i].RoBindex<<" vj:"<<LSB_list[i].vj<<" qj:"<<LSB_list[i].qj<<" vk:"<<LSB_list[i].vk<<" qk:"<<LSB_list[i].qk<<" imm:"<<LSB_list[i].imm<<std::endl;
                }
            }else{
                for(int i=beg+1;i<=end+LSBsize;i++){
                    std::cout<<LSB_list[i%LSBsize].op<<" index in RoB:"<<LSB_list[i%LSBsize].RoBindex<<" vj:"<<LSB_list[i%LSBsize].vj<<" qj:"<<LSB_list[i%LSBsize].qj<<" vk:"<<LSB_list[i%LSBsize].vk<<" qk:"<<LSB_list[i%LSBsize].qk<<" imm:"<<LSB_list[i%LSBsize].imm<<std::endl;
                }
            }
        }
    }

    void add(InstructionUnit Ins,int index,RegisterFile *RF,CDB *cdb){
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
                        if(cdb->num==1){
                            if(cdb->update.RoB_index==tmp.qj){
                                tmp.vj=cdb->update.value;
                                tmp.qj=-1;
                            }
                        }
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
                    }else{//否则，将依赖的编号存入qj(注意同一周期内更新的情况)
                        tmp.qj=RF->regs[Ins.rs1].rely;
                        if(cdb->num==1){
                            if(cdb->update.RoB_index==tmp.qj){
                                tmp.vj=cdb->update.value;
                                tmp.qj=-1;
                            }
                        }
                    }

                    if(!RF->regs[Ins.rs2].busy){//如果没有依赖，直接读取数值
                        tmp.vk=RF->regs[Ins.rs2].data;
                        tmp.qk=-1;
                    }else{//否则，将依赖的编号存入qk
                        tmp.qk=RF->regs[Ins.rs2].rely;
                        if(cdb->num==1){
                            if(cdb->update.RoB_index==tmp.qk){
                                tmp.vk=cdb->update.value;
                                tmp.qk=-1;
                            }
                        }
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

    void broadcast(CDB_value res){
        if(LSB_list_next.empty()){
            return;
        }
        int beg=LSB_list.front;
        int end=LSB_list.rear;
        if(end>beg){
            for(int i=beg+1;i<=end;i++){
                if(LSB_list[i].qj==res.RoB_index){//如果qj的依赖正好是广播的指令，则更新依赖
                    LSB_list_next[i].qj=-1;
                    LSB_list_next[i].vj=res.value;
                }
                if(LSB_list[i].qk==res.RoB_index){
                    LSB_list_next[i].qk=-1;
                    LSB_list_next[i].vk=res.value;
                }
            }
        }else{
            for(int i=beg+1;i<=end+LSBsize;i++){
                if(LSB_list[i%LSBsize].qj==res.RoB_index){//如果qj的依赖正好是广播的指令，则更新依赖
                    LSB_list_next[i%LSBsize].qj=-1;
                    LSB_list_next[i%LSBsize].vj=res.value;
                }
                if(LSB_list[i%LSBsize].qk==res.RoB_index){
                    LSB_list_next[i%LSBsize].qk=-1;
                    LSB_list_next[i%LSBsize].vk=res.value;
                }
            }
        }
    }

    void execute(Memory *mem,ReorderBuffer *RoB,CDB *cdb){
        if(cdb->num==1){
            CDB_value newinf=cdb->update;
            broadcast(newinf);
        }
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
//                        std::cout<<"从 "<<addr<<" 处载入 "<<result<<"\n";
                        RoB->finish_calc(obj.RoBindex,result);
                        LSB_list_next.pop();
                    }
                    break;
                }
               case Sb:
               case Sh:
               case Sw:{
                   if(obj.qj==-1&&obj.qk==-1){
                       int addr=obj.vj+obj.imm;
                       RoB->finish_calc(obj.RoBindex,addr);
                       LSB_list_next.pop();
                   }
                   break;
               }
                default:
                    break;
            }
        }
    }
};
#endif //CODE_LOADSTOREBUFFER_H
