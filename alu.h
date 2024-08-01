//
// Created by 24115 on 2024/7/30.
//

#ifndef CODE_ALU_H
#define CODE_ALU_H
#pragma once
#include "decode.h"
class ALU{
public:
    int execute(Instr op,int rs1,int rs2){
        switch (op) {
            case Beq:return BEQ(rs1,rs2);
            case Bne:return BNE(rs1,rs2);
            case Blt:return BLT(rs1,rs2);
            case Bge:return BGE(rs1,rs2);
            case Bltu:return BLTU(rs1,rs2);
            case Bgeu:return BGEU(rs1,rs2);
            case Addi:return ADDI(rs1,rs2);
            case Slti:return SLTI(rs1,rs2);
            case Sltiu:return SLTIU(rs1,rs2);
            case Andi:return ANDI(rs1,rs2);
            case Ori:return ORI(rs1,rs2);
            case Xori:return XORI(rs1,rs2);
            case Slli:return SLLI(rs1,rs2);
            case Srli:return SRLI(rs1,rs2);
            case Srai:return SRAI(rs1,rs2);
            case Add:return ADD(rs1,rs2);
            case Slt:return SLT(rs1,rs2);
            case Sltu:return SLTU(rs1,rs2);
            case And:return AND(rs1,rs2);
            case Or:return OR(rs1,rs2);
            case Xor:return XOR(rs1,rs2);
            case Sll:return SLL(rs1,rs2);
            case Srl:return SRL(rs1,rs2);
            case Sra:return SRA(rs1,rs2);
            case Sub:return SUB(rs1,rs2);
        }
    }

    int BEQ(int rs1,int rs2){
        if(rs1==rs2){
            return 1;
        }else{
            return 0;
        }
    }

    int BNE(int rs1,int rs2){
        if(rs1!=rs2){
            return 1;
        }else{
            return 0;
        }
    }

    int BLT(int rs1,int rs2){
        if(rs1<rs2){
            return 1;
        }else{
            return 0;
        }
    }

    int BGE(int rs1,int rs2){
        if(rs1>rs2){
            return 1;
        }else{
            return 0;
        }
    }

    int BLTU(int rs1,int rs2){
        if(static_cast<unsigned int>(rs1)<static_cast<unsigned int>(rs2)){
            return 1;
        }else{
            return 0;
        }
    }

    int BGEU(int rs1,int rs2){
        if(static_cast<unsigned int>(rs1)>static_cast<unsigned int>(rs2)){
            return 1;
        }else{
            return 0;
        }
    }

    int ADDI(int rs1,int imm){
        return rs1+imm;
    }

    int SLTI(int rs1,int imm){
        return rs1<imm;
    }

    int SLTIU(int rs1,int imm){
        return static_cast<unsigned int>(rs1)<static_cast<unsigned int>(imm);
    }

    int ANDI(int rs1,int imm){
        return rs1&imm;
    }

    int ORI(int rs1,int imm){
        return rs1|imm;
    }

    int XORI(int rs1,int imm){
        return rs1^imm;
    }

    int SLLI(int rs1,int shamt){
        return rs1<<shamt;
    }

    int SRLI(int rs1,int shamt){//逻辑右移，需要先转换成unsigned
        return static_cast<unsigned int>(rs1)>>shamt;
    }

    int SRAI(int rs1,int shamt){//算术右移
        return rs1>>shamt;
    }

    int ADD(int rs1,int rs2){
        return rs1+rs2;
    }

    int SLT(int rs1,int rs2){
        return rs1<rs2;
    }

    int SLTU(int rs1,int rs2){
        return static_cast<unsigned int>(rs1)<static_cast<unsigned int>(rs2);
    }

    int AND(int rs1,int rs2){
        return rs1&rs2;
    }

    int OR(int rs1,int rs2){
        return rs1|rs2;
    }

    int XOR(int rs1,int rs2){
        return rs1^rs2;
    }

    int SLL(int rs1,int rs2){
        return rs1<<rs2;
    }

    int SRL(int rs1,int rs2){
        return static_cast<unsigned int>(rs1)>>rs2;
    }

    int SRA(int rs1,int rs2){
        return rs1>>rs2;
    }

    int SUB(int rs1,int rs2){
        return rs1-rs2;
    }
};

#endif //CODE_ALU_H
