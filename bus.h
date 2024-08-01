//
// Created by 24115 on 2024/7/31.
//

#ifndef CODE_BUS_H
#define CODE_BUS_H
//构建各元件之间数据交流的结构体

struct CDB_value{
    int value;//计算出来的结果
    int RoB_index;//在RoB中的编号
    int index;//寄存器的编号
};

struct RoBentry{
    RoBtype type;
    int index;//在RoB中的编号
    InstructionUnit Itr;
    bool ready;
    int dest;//对应的目标寄存器
    int value;//计算出来的值
    int addr;//记录可能跳转的地址（用于branch指令）
    int prediction;//用于branch指令的分支预测
};

struct RSunit{
    int RoBindex;//在RoB中的编号
    bool busy=false;
    Instr op;
    int vj,vk;
    int qj,qk;
    int result;
};

struct LSBentry{
    Instr op;
    int RoBindex;
    int vj,vk;
    int qj=-1,qk=-1;
    unsigned int imm;
    int A;//Load或Store的地址
};
#endif //CODE_BUS_H
