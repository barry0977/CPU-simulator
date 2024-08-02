//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_DECODE_H
#define CODE_DECODE_H

enum RoBtype{//传入RoB的指令类型
    toreg,
    store_,
    load_,
    branch_,
    else_,
    exit_,
};

enum Instr{//所有指令
    Lui,//0
    Auipc,//1
    Jal,//2
    Jalr,//3
    Beq,//4
    Bne,//5
    Blt,//6
    Bge,//7
    Bltu,//8
    Bgeu,//9
    Lb,//10
    Lh,//11
    Lw,//12
    Lbu,//13
    Lhu,//14
    Sb,//15
    Sh,//16
    Sw,//17
    Addi,//18
    Slti,//19
    Sltiu,//20
    Xori,//21
    Ori,//22
    Andi,//23
    Slli,//24
    Srli,//25
    Srai,//26
    Add,//27
    Sub,//28
    Sll,//29
    Slt,//30
    Sltu,//31
    Xor,//32
    Srl,//33
    Sra,//34
    Or,//35
    And,//36
    Exit//37
};

struct InstructionUnit{//一条指令所包含的信息
    Instr ins;
    unsigned int rs1,rs2;
    unsigned int rd;//目标寄存器
    unsigned int imm;
    unsigned int PC;
    bool isjump;
};

InstructionUnit decode(int ins,unsigned int PC){
    InstructionUnit ans;
    Instr op;
    unsigned int rs1,rs2,rd,imm;
    rs1=(ins>>15)&0b11111;
    rs2=(ins>>20)&0b11111;
    rd=(ins>>7)&0b11111;
    int funct3=(ins>>12)&0b111;
    int funct7=(ins>>25)&0b1111111;

    int type=ins&(0b1111111);
    //先获得立即数
    if(type==0b0110111||type==0b0010111){  //U
        imm=ins&(0xFFFFF000);
        if(type==0b0110111){
            op=Lui;
        }else{
            op=Auipc;
        }
    }else if(type==0b1101111){  //J
        imm=(((ins>>31)&0b1)<<20)|(((ins>>21)&0b1111111111)<<1)|(((ins>>20)&1)<<11)|(((ins>>12)&0b11111111)<<12);
        imm=static_cast<int>(imm<<11)>>11;//符号扩展
        op=Jal;
    }else if(type==0b1100111||type==0b0000011||type==0b0010011){  //I
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
        }else if(type == 0b0010011){
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
    }else if(type==0b1100011){  //B
        imm=(((ins>>31)&0b1)<<12)|(((ins>>25)&0b111111)<<5)|(((ins>>8)&0b1111)<<1)|(((ins>>7)&0b1)<<11);
        imm=static_cast<int>(imm<<19)>>19;//符号扩展

        rd=-1;//B指令没有目的寄存器

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
    }else if(type==0b0100011){  //S
        imm=(((ins>>25)&0b1111111)<<5)|((ins>>7)&0b11111);
        imm=static_cast<int>(imm<<20)>>20;

        rd=-1;//S指令没有目的寄存器

        if(funct3==0b000){
            op=Sb;
        }else if(funct3==0b001){
            op=Sh;
        }else if(funct3==0b010){
            op=Sw;
        }
    }else if(type==0b0110011){  //R
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
        op=Exit;
    }

    ans.ins=op;
    ans.rs1=rs1;
    ans.rs2=rs2;
    ans.imm=imm;
    ans.rd=rd;
    ans.PC=PC;
    return ans;
}

RoBtype get_RoBtype(Instr op){
    RoBtype ans;
    switch (op) {
        case Lui:
        case Auipc:
        case Jal:
        case Jalr:ans=else_;
            break;
        case Lb:
        case Lh:
        case Lw:
        case Lbu:
        case Lhu:ans=load_;
            break;
        case Addi:
        case Slti:
        case Sltiu:
        case Xori:
        case Ori:
        case Andi:
        case Slli:
        case Srli:
        case Srai:
        case Add:
        case Sub:
        case Sll:
        case Slt:
        case Sltu:
        case Xor:
        case Srl:
        case Sra:
        case Or:
        case And: ans=toreg;
            break;
        case Beq:
        case Bne:
        case Blt:
        case Bge:
        case Bltu:
        case Bgeu:ans=branch_;
            break;
        case Sb:
        case Sh:
        case Sw:ans=store_;
            break;
        case Exit:ans=exit_;
            break;
    }
    return ans;
};

int rs_cnt(Instr op){//需要的寄存器个数
    int ans=0;
    switch (op) {
        case Lui:
        case Auipc:
        case Jal:ans=0;
            break;
        case Jalr:ans=1;
            break;
        case Lb:
        case Lh:
        case Lw:
        case Lbu:
        case Lhu:ans=1;
            break;
        case Addi:
        case Slti:
        case Sltiu:
        case Xori:
        case Ori:
        case Andi:
        case Slli:
        case Srli:
        case Srai:ans=1;
            break;
        case Add:
        case Sub:
        case Sll:
        case Slt:
        case Sltu:
        case Xor:
        case Srl:
        case Sra:
        case Or:
        case And: ans=2;
            break;
        case Beq:
        case Bne:
        case Blt:
        case Bge:
        case Bltu:
        case Bgeu:ans=2;
            break;
        case Sb:
        case Sh:
        case Sw:ans=2;
            break;
        case Exit:ans=0;
            break;
    }
    return ans;
}

// 定义字符串数组
const std::string InstrNames[] = {
        "LUI",    // 0
        "AUIPC",  // 1
        "JAL",    // 2
        "JALR",   // 3
        "BEQ",    // 4
        "BNE",    // 5
        "BLT",    // 6
        "BGE",    // 7
        "BLTU",   // 8
        "BGEU",   // 9
        "LB",     // 10
        "LH",     // 11
        "LW",     // 12
        "LBU",    // 13
        "LHU",    // 14
        "SB",     // 15
        "SH",     // 16
        "SW",     // 17
        "ADDI",   // 18
        "SLTI",   // 19
        "SLTIU",  // 20
        "XORI",   // 21
        "ORI",    // 22
        "ANDI",   // 23
        "SLLI",   // 24
        "SRLI",   // 25
        "SRAI",   // 26
        "ADD",    // 27
        "SUB",    // 28
        "SLL",    // 29
        "SLT",    // 30
        "SLTU",   // 31
        "XOR",    // 32
        "SRL",    // 33
        "SRA",    // 34
        "OR",     // 35
        "AND",    // 36
        "EXIT"    // 37
};
#endif //CODE_DECODE_H
