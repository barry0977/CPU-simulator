//
// Created by 24115 on 2024/7/30.
//

#ifndef CODE_INSTRUCTIONQUEUE_H
#define CODE_INSTRUCTIONQUEUE_H
#include "memory.h"
//#include "decode.h"
#include "myqueue.h"
//#include "bus.h"

class InstructionQueue{
private:
    Queue<InstructionUnit,64>IQ_next;
    int PC_next;
    bool pause_next=false;
    Memory *mem;
public:
    Queue<InstructionUnit,64>IQ;
    int PC;
    bool pause= false;//是否暂停

public:
    void init(Memory *memory){
        PC=0;
        PC_next=4;
        mem=memory;
    }

    void refresh(){
        PC=PC_next;
        IQ=IQ_next;
    }

    void set_PC(unsigned int addr){
        PC_next=addr;
        pause=false;
    }

    void IF(){
        unsigned int itr=0;
        PC;
        itr=mem->read_word(PC);
        if(itr==0){
            pause_next=true;
            return;
        }
        InstructionUnit Ins= decode(itr,PC);
        std::cout<<"当前获取的指令："<<Ins.ins<<" rs1:"<<Ins.rs1<<" rs2:"<<Ins.rs2<<" rd:"<<Ins.rd<<" imm:"<<Ins.imm<<" PC:"<<Ins.PC<<"\n";
        IQ_next.push(Ins);
        if(Ins.ins==Jal){
            PC_next=PC+Ins.imm;
        }else if(Ins.ins==exit_){
            pause=true;
        }else if(Ins.ins==Jalr){
            pause=true;
        }else{
            PC_next=PC+4;
        }
    }

    void execute(ReorderBuffer *RoB,ReservationStation *RS,LoadStoreBuffer *LSB,RegisterFile *RF){
        if(!pause){
            IF();
        }
        launch(RoB,RS,LSB,RF);
    }

    void launch(ReorderBuffer *RoB,ReservationStation *RS,LoadStoreBuffer *LSB,RegisterFile *RF){
        if(IQ.empty()){
            return;
        }
        InstructionUnit ins=IQ.top();//取出Instruction Queue队头的元素
        RoBtype type= get_RoBtype(ins.ins);
        if(type==store_||type==load_){
            if(RoB->available()&&LSB->available()){//如果都可用，则加入
                int index=RoB->issue(ins,RF);
                LSB->add(ins,index,RF);
                IQ_next.pop();
            }
        }else{
            //Lui和Auipc不用放进RS,直接计算,RoB里面设置成ready
            //latest: Lui和Auipc也放进RS,方便广播
            if(ins.ins==Lui){
                if(RoB->available()){
                    RoBentry tmp;
                    tmp.ready=false;
                    tmp.dest=ins.rd;
                    tmp.value=ins.imm;
                    tmp.type=type;
                    tmp.Itr=ins;
                    int index=RoB->issue(tmp,RF);
                    RS->add(ins,index,RF);
                    IQ_next.pop();
                }
            }else if(ins.ins==Auipc){
                if(RoB->available()){
                    RoBentry tmp;
                    tmp.ready=false;
                    tmp.dest=ins.rd;
                    tmp.value=ins.imm+ins.PC;
                    tmp.type=type;
                    tmp.Itr=ins;
                    int index=RoB->issue(tmp,RF);
                    RS->add(ins,index,RF);
                    IQ_next.pop();
                }
            }else if(ins.ins==Jal){
                if(RoB->available()){
                    RoBentry tmp;
                    tmp.ready=false;
                    tmp.dest=ins.rd;
                    tmp.value=ins.PC+4;
                    tmp.type=type;
                    tmp.Itr=ins;
                    int index=RoB->issue(tmp,RF);
                    RS->add(ins,index,RF);
                    IQ_next.pop();
                }
            }else if(ins.ins==Jalr){
                if(RoB->available()&&RS->available()){
                    RoBentry tmp;
                    tmp.ready=false;
                    tmp.dest=ins.rd;
                    tmp.value=ins.PC+4;
                    tmp.type=type;
                    tmp.Itr=ins;
                    int index=RoB->issue(tmp,RF);
                    RS->add_jalr(ins,index,RF);
                    IQ_next.pop();
                }
            }else if(type==branch_){
                if(RoB->available()&&RS->available()){
                    RoBentry tmp;
                    tmp.ready=false;
                    tmp.addr=ins.PC+ins.imm;//设置好跳转的地址
                    tmp.Itr=ins;
                    tmp.type=type;
                    tmp.dest=-1;//没有目标寄存器
                    int index=RoB->issue(tmp,RF);
                    RS->add(ins,index,RF);
                    IQ_next.pop();
                }
            }else{
                if(RoB->available()&&RS->available()){
                    int index=RoB->issue(ins,RF);
                    RS->add(ins,index,RF);
                    IQ_next.pop();
                }
            }
        }
    }
};

#endif //CODE_INSTRUCTIONQUEUE_H
