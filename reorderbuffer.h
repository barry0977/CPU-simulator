//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REORDERBUFFER_H
#define CODE_REORDERBUFFER_H
#include "myqueue.h"
#include "instructions.h"

struct RoBentry{
    RoBtype type;
    int index;
    InstructionUnit Itr;
    bool ready;
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

    void launch(InstructionUnit ins){
        if(list.full()){
            return;
        }else{
            int index=list.get_tail();
            RoBentry tmp;

        }
    }

    void commit(){
        RoBentry top=list.top();
        if(top.ready){
            list_next.pop();
            if(top.type==toreg){

            }else if(top.type==tomem){

            }else if(top.type==tobranch){

            }else if(top.type==exit_){

            }
        }
    }
};

#endif //CODE_REORDERBUFFER_H
