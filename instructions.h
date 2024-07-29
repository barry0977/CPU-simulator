
//
// Created by 24115 on 2024/7/29.
//

#ifndef CODE_INSTRUCTIONS_H
#define CODE_INSTRUCTIONS_H
#include "memory.h"

enum Instr{
    Lui,
    Auipc,
    Jal,
    Jalr,
    Beq,
    Bne,
    Blt,
    Bge,
    Bltu,
    Bgeu,
    Lb,
    Lh,
    Lw,
    Lbu,
    Lhu,
    Sb,
    Sh,
    Sw,
    Addi,
    Slti,
    Sltiu,
    Xori,
    Ori,
    Andi,
    Slli,
    Srli,
    Srai,
    Add,
    Sub,
    Sll,
    Slt,
    Sltu,
    Xor,
    Srl,
    Sra,
    Or,
    And,
    End
};

class InstructionUnit{
public:
    Instr ins;
    int rs1,rs2;
    int rd;
    unsigned int imm;
    unsigned int PC;

public:
    void execute(Memory *mem){
        int tmp;
        mem->read_word(tmp,PC);

    }
};

#endif //CODE_INSTRUCTIONS_H
