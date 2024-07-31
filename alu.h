//
// Created by 24115 on 2024/7/30.
//

#ifndef CODE_ALU_H
#define CODE_ALU_H
#pragma once
class ALU{
public:
    unsigned execute(){

    }

    unsigned int LUI(unsigned int imm){
        return imm;
    }

    unsigned int AUIPC(unsigned int PC,unsigned int imm){
        return PC+imm;
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

    int SLTI(int rs1,nt imm){
        return
    }

    int SLTIU()
};

#endif //CODE_ALU_H
