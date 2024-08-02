//
// Created by 24115 on 2024/7/30.
//

#ifndef CODE_PREDICTOR_H
#define CODE_PREDICTOR_H

class Predictor{
public:
    int pred[4096]={};

    bool predict(int addr){
        int index=addr/4;
        if(pred[index]<=1){
            return false;
        }else{
            return true;
        }
    }

    void update_predict(int addr, bool isjump){
        int index=addr/4;
        if(isjump){//跳转了
            if(pred[index]!=0b11){
                pred[index]++;
            }
        }else{//没有跳转
            if(pred[index]!=0b00){
                pred[index]--;
            }
        }
    }
};

#endif //CODE_PREDICTOR_H
