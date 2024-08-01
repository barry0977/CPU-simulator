//
// Created by 24115 on 2024/8/1.
//

#ifndef CODE_CDB_H
#define CODE_CDB_H
#include "bus.h"
//用于RS和LSB之间相互传输数据
enum client{
    toRS,
    toLSB,
};

struct package{
    client to;//交给谁
    CDB_value data;
    bool has=false;//是否有数据
};

class CDB{
public:
    package st[2];
    int cnt=0;

    void refresh(){
        cnt=0;
    }

    void send_value(CDB_value obj,client towho){
        cnt++;
        st[cnt-1].data=obj;
        st[cnt-1].to=towho;
        st[cnt-1].has=true;
    }

    package get_value(client towho){
        for(int i=0;i<cnt;i++){
            if(st[i].to==towho){
                return st[i];
            }
        }
        return package();
    }
};

#endif //CODE_CDB_H
