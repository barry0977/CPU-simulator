//
// Created by 24115 on 2024/7/28.
//

#ifndef CODE_MYQUEUE_H
#define CODE_MYQUEUE_H
template<typename T,int cap>
class Queue{
public:
    T value[cap];
    int front=0;
    int rear=0;
public:
    void push(T data){
        rear=(rear+1)%cap;
        value[rear]=data;
    }

    int get_head(){
        return (front+1)%cap;
    }

    int get_tail(){
        return (rear+1)%cap;
    }

    T pop(){
        front=(front+1)%cap;
        return value[front];
    }

    T top(){
        return value[(front+1)%cap];
    }

    bool full(){
        if((rear+1)%cap==front){
            return true;
        }else{
            return false;
        }
    }

    bool empty(){
        return front==rear;
    }

    T& operator[](int index){
        return value[index];
    }

    void clear(){
        rear=front;
    }
};

#endif //CODE_MYQUEUE_H
