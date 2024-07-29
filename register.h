//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_REGISTER_H
#define CODE_REGISTER_H
class Register
{
private:
    int data_next;
    int rely_next;
    bool busy_next;
public:
    int data;
    int rely;
    bool busy;
public:
    void refresh(){
        data=data_next;
        rely=rely_next;
        busy=busy_next;
        data_next=0;
        rely_next=0;
        busy_next= false;
    }
};
#endif //CODE_REGISTER_H
