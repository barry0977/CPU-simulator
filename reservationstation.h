//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_RESERVATIONSTATION_H
#define CODE_RESERVATIONSTATION_H
#include "instructions.h"
struct RSunit{
    int RoBindex;
    bool busy;
    Instr op;
    int vj,vk;
    int qj,qk;
    int A;
};

class ReservationStation{
private:
    RSunit rs_next[16];
public:
    RSunit rs[16];
public:
    void refresh(){
        for(int i=0;i<16;i++){
            rs[i]=rs_next[i];
        }
    }

    void add(InstructionUnit ins){
        int index=-1;
        for(int i=0;i<16;i++){
            if(!rs[i].busy){
                index=i;
                break;
            }
        }

    }

    void execute(){

    }
};

#endif //CODE_RESERVATIONSTATION_H
