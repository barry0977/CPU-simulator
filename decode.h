//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_DECODE_H
#define CODE_DECODE_H
#include "instructions.h"

const int opcode=0b1111111;
const int imm_31_25=0b1111111<<25;
const int rd_=0b11111<<7;

InstructionUnit decode(int ins){
    InstructionUnit ans;
    Instr op;
    int rs1,rs2,imm;
    rs1=(ins>>15)&0b11111;
    rs2=(ins>>20)&0b11111;
    int funct3=(ins>>12)&0b111;
    int funct7=(ins>>25)&0b1111111;

    int type=ins&(0b1111111);
    //先获得立即数
    if(type==0b0110111||type==0b0010111){//U
        imm=ins&(~0b111111111111u);
        if(type==0b0110111){
            op=Lui;
        }else{
            op=Auipc;
        }
    }else if(type==0b1101111){//J
        imm=(((ins>>31)&0b1)<<20)|(((ins>>21)&0b1111111111)<<1)|(((ins>>20)&1)<<11)|(((ins>>12)&0b11111111)<<12);
        imm=static_cast<int>(imm<<11)>>11;//符号扩展
        op=Jal;
    }else if(type==0b1100111||type==0b0000011||type==0b0010011){//I
        imm=(ins>>20);
        if(type==0b1100111){
            op=Jalr;
        }else if(type==0b0000011){
            if(funct3==0b000){
                op=Lb;
            }else if(funct3==0b001){
                op=Lh;
            }else if(funct3==0b010){
                op=Lw;
            }else if(funct3==0b100){
                op=Lbu;
            }else if(funct3==0b101){
                op=Lhu;
            }
        }else if(type==0b0010011){
            if(funct3==0b000){
                op=Addi;
            }else if(funct3==0b010){
                op=Slti;
            }else if(funct3==0b011){
                op=Sltiu;
            }else if(funct3==0b100){
                op=Xori;
            }else if(funct3==0b110){
                op=Ori;
            }else if(funct3==0b111){
                op=Andi;
            }else if(funct3==0b001){
                op=Slli;
            }else if(funct3==0b101){
                if((ins>>30)&0b1){
                    op=Srai;
                }else{
                    op=Srli;
                }
            }
        }
    }else if(type==0b1100011){//B
        imm=(((ins>>31)&0b1)<<12)|(((ins>>25)&0b111111)<<5)|(((ins>>8)&0b1111)<<1)|(((ins>>7)&0b1)<<11);
        imm=static_cast<int>(imm<<19)>>19;//符号扩展

        if(funct3==0b000){
            op=Beq;
        }else if(funct3==0b001){
            op=Bne;
        }else if(funct3==0b100){
            op=Blt;
        }else if(funct3==0b101){
            op=Bge;
        }else if(funct3==0b110){
            op=Bltu;
        }else if(funct3==0b111){
            op=Bgeu;
        }
    }else if(type==0b0100011){//S
        imm=((ins>>25)&0b1111111<<5)|((ins>>7)&0b11111);
        imm=static_cast<int>(imm<<20)>>20;

        if(funct3==0b000){
            op=Sb;
        }else if(funct3==0b001){
            op=Sh;
        }else if(funct3==0b010){
            op=Sw;
        }
    }else if(type==0b0110011){//R
        imm=0;

        if(funct3==0b000){
            if((ins>>30)&0b1){
                op=Sub;
            }else{
                op=Add;
            }
        }else if(funct3==0b001){
            op=Sll;
        }else if(funct3==0b010){
            op=Slt;
        }else if(funct3==0b011){
            op=Sltu;
        }else if(funct3==0b100){
            op=Xor;
        }else if(funct3==0b101){
            if((ins>>30)&0b1){
                op=Sra;
            }else{
                op=Srl;
            }
        }else if(funct3==0b110){
            op=Or;
        }else if(funct3==0b111){
            op=And;
        }
    }

    if(ins==0x0ff00513){
        op=End;
    }

    ans.ins=op;
    ans.rs1=rs1;
    ans.rs2=rs2;

}


#endif //CODE_DECODE_H
