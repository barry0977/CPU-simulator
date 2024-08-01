//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REORDERBUFFER_H
#define CODE_REORDERBUFFER_H
#include "myqueue.h"
#include "decode.h"
#include "bus.h"
#include "registerfile.h"

class ReorderBuffer{
private:
    Queue<RoBentry,20>list_next;
public:
    Queue<RoBentry,20>list;
public:
    void refresh(){
        list=list_next;
    }

    bool available(){//是否可用
        if(list.full()){
            return false;
        }else{
            return true;
        }
    }

    void show(){
        std::cout<<"----------- RoB ---------------------\n";
        if(list.empty()){
            std::cout<<"RoB is empty\n";
        }else{
            int beg=(list.front+1)%20;
            for(int i=beg;i<=list.rear;i=(i+1)%20){
                std::cout<<list[i].Itr.ins<<" dest:"<<list[i].dest<<" value:"<<list[i].value<<" ready:"<<list[i].ready<<std::endl;
            }
        }
    }

    int issue(InstructionUnit ins,RegisterFile *RF){
        if(list.full()){
            std::cout<<"RoB is full\n";
            return -1;
        }else{
            int index=list.get_tail();
            RoBentry tmp;
            tmp.type= get_RoBtype(ins.ins);
            tmp.dest=ins.rd;
            tmp.ready=false;
            tmp.index=index;
            tmp.Itr=ins;
            if(tmp.dest>=0){
                RF->add_dependency(tmp.dest,tmp.index);
            }
            list_next.push(tmp);
            return index;
        }
    }

    int issue(RoBentry res,RegisterFile *RF){
        if(list.full()){
            std::cout<<"RoB is full\n";
            return -1;
        }else{
            int index=list.get_tail();
            res.index=index;
            if(res.dest>=0){
                RF->add_dependency(res.dest,index);
            }
            list_next.push(res);
            return index;
        }
    }

    void flush(){//清空RoB
        list_next.clear();
    }

    void execute(RegisterFile *RF,Memory *mem){
        commit(RF,mem);
    }

    void finish_calc(int index,int value){
        list_next[index].ready= true;
        list_next[index].value=value;
    }

    void finish_jalr(int index,int value){
        list_next[index].ready=true;
        list_next[index].addr=value;
    }

    int commit(RegisterFile *RF,Memory *mem){
        RoBentry top=list.top();
        if(top.ready){
            list_next.pop();
            if(top.type==toreg){
                CDB_value tmp;
                tmp.value=top.value;
                tmp.RoB_index=top.index;
                RF->update_data(top.Itr.rd,tmp);
            }else if(top.type==else_) {
                if(top.Itr.ins==Jalr) {
                    CDB_value tmp;
                    tmp.value=top.value;
                    tmp.RoB_index=top.index;
                    RF->update_data(top.Itr.rd,tmp);
                    return top.addr;
                }else {
                    CDB_value tmp;
                    tmp.value=top.value;
                    tmp.RoB_index=top.index;
                    RF->update_data(top.Itr.rd,tmp);
                }
            }else if(top.type==store_){//当store被commit时，前面都已经commit，可以直接从RF中读取
                int rs1=RF->regs[top.Itr.rs1].data,rs2=RF->regs[top.Itr.rs2].data;
                int data=rs2&0xFF,index=rs1+top.Itr.imm;
                mem->store(data,index,top.Itr.ins);
            }else if(top.type==load_){
                CDB_value tmp;
                tmp.value=top.value;
                tmp.RoB_index=top.index;
                RF->update_data(top.Itr.rd,tmp);
            }else if(top.type==branch_){
                if(top.value==1){//需要跳转
                    return top.addr;//返回需要跳转的地址
                }
            }else if(top.type==exit_){
                std::cout<<static_cast<unsigned int>(RF->regs[10].data&0xFF)<<std::endl;
                exit(0);
            }
        }
        return -1;
    }
};

#endif //CODE_REORDERBUFFER_H
