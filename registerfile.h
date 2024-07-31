//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REGISTERFILE_H
#define CODE_REGISTERFILE_H

struct Register{
    int data=0;//储存的数据
    int rely=-1;//最新值将由哪条指令算出
    bool busy=false;//是否有依赖
};

class RegisterFile{
private:
    Register regs_next[32];
public:
    Register regs[32];
public:

    void refresh(){
        for(int i=1;i<32;i++){
            regs[i]=regs_next[i];
            regs_next[i]=regs[i];
        }
        regs[0].data=0;
        regs[0].rely=-1;
        regs[0].busy=false;
    }

    void add_dependency(int index,int rely){
        if(index==0){//忽略对x0寄存器的修改
            return;
        }
        regs_next[index].busy=true;
        regs_next[index].data=regs[index].data;
        regs_next[index].rely=rely;
    }

    void update_data(int index,CDB_value res){
        if(index==0){//忽略对x0寄存器的修改
            return;
        }
        regs_next[index].data=res.value;
        if(res.RoB_index==regs[index].rely){
            regs_next[index].busy=false;
            regs_next[index].rely=-1;
        }
    }
};

#endif //CODE_REGISTERFILE_H