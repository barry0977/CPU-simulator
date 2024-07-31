//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REORDERBUFFER_H
#define CODE_REORDERBUFFER_H
#include "myqueue.h"
#include "decode.h"
#include "registerfile.h"
struct RoBentry{
    RoBtype type;
    int index;//在RoB中的编号
    InstructionUnit Itr;
    bool ready;
    int value;
};

class ReorderBuffer{
private:
    Queue<RoBentry,20>list_next;
public:
    Queue<RoBentry,20>list;
public:
    void refresh(){
        list=list_next;
    }

    int issue(InstructionUnit ins,RegisterFile *RF){
        if(list.full()){
            std::cout<<"RoB is full\n";
            return -1;
        }else{
            int index=list.get_tail();
            RoBentry tmp;
            tmp.type= get_RoBtype(ins.ins);
            tmp.ready=false;
            tmp.index=index;
            tmp.Itr=ins;
            list_next[index]=tmp;
            return index;
        }
    }

    void finish_calc(int index,int value){

    }

    void commit(RegisterFile *RF){
        RoBentry top=list.top();
        if(top.ready){
            list_next.pop();
            if(top.type==toreg){
                RF->update_data(top.Itr.rd,top.index,top.value);
            }else if(top.type==store_){

            }else if(top.type==load_){

            }else if(top.type==br_succeed){

            }else if(top.type==br_fail){

            }else if(top.type==exit_){

            }
        }
    }
};

#endif //CODE_REORDERBUFFER_H
