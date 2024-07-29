//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_CPU_H
#define CODE_CPU_H
#include "reorderbuffer.h"
#include "reservationstation.h"
#include "registerfile.h"
class CPU{
private:
    ReorderBuffer RB;
    ReservationStation RS;
    RegisterFile RF;
};

#endif //CODE_CPU_H
