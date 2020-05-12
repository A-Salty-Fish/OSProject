//
// Created by dzy on 2020/5/10.
//
#include <iostream>
#include <time.h>
#include <deque>
#ifndef PROJECT5_PRODUCERCONSUMER_H
#define PROJECT5_PRODUCERCONSUMER_H
using namespace std;
enum STATUS{RUNNING,READY,BLOCK,END};//进程状态
enum WAITINGREASON{None,S1,S2};//等待原因

typedef int semaphore;

class PCB{//进程控制块
public:
    string name;//进程名
    STATUS status;//进程状态
    WAITINGREASON waitingReason;//进程等待原因
    int Breakpoint;//进程断点
    PCB():status(STATUS::READY),waitingReason(WAITINGREASON::None){}
};

class Process{
public:
    PCB pcb;
    int count;//记录调用次数
    Process(string name):count(0){pcb.name=name;}
    bool IsEnd(){return pcb.status==STATUS::END;}//判断进程是否结束
};

class ProcessProducer:public Process{//生产者进程
    int Product;//产品
public:
    enum PRODUCER{//生产者指令 PA
        PRODUCE=0,
        P_S1_,
        PUT,
        V_S2_,
        GO_TO_0
    };
    ProcessProducer(string name):Process(name),Product(0)
    {pcb.Breakpoint=PRODUCER::PRODUCE;}//默认断点为第一条命令

    virtual bool P(semaphore &s);//生产者P操作
    virtual WAITINGREASON V(semaphore &s);//生产者V操作
    void Produce();//生产函数
    void Put(deque<int>& Buffer);//放入缓冲区
};

bool ProcessProducer::P(semaphore &s) {//生产者P操作 返回是否阻塞
    if (pcb.status == BLOCK)//如果是从阻塞中唤醒
        pcb.status == READY;
    else
        s--;//信号量减一
    cout << "Producer P(S1) : " ;
    if (s<0)//信号量小于零则阻塞
    {
        cout<<"Block"<<endl;
        pcb.status=STATUS::BLOCK;
        pcb.waitingReason=WAITINGREASON::S1;
        return true;
    }
    else
    {
        cout<<"Continue"<<endl;
        pcb.status=STATUS::READY;
        pcb.waitingReason=WAITINGREASON::None;
        return false;
    }
}

WAITINGREASON ProcessProducer::V(semaphore &s) {//生产者V操作 返回唤醒S2
    s++;//信号量加一
    cout << "Producer V(S2) : " ;
    if (s<=0)//信号量小于1则需要唤醒
    {
        cout<<"wake up"<<endl;
        return S2;
    }
    else
    {
        cout<<"continue"<<endl;
        return None;
    }
}

void ProcessProducer::Produce() {//生产函数
    Product=rand()%10+1;
    cout<<"Producer Produce : "<<Product<<endl;
}

void ProcessProducer::Put(deque<int>& Buffer) {
    Buffer.push_back(Product);
    cout<<"Producer Put : "<<Product<<endl;
}

class ProcessConsumer:public Process{//消费者进程
    int Product;//产品
public:
    enum CONSUMER{//消费者指令 SA
        P_S2_=6,
        GET,
        V_S1_,
        CONSUME,
        GO_TO_0
    };
    ProcessConsumer(string name):Process(name),Product(0)
    {pcb.Breakpoint=CONSUMER::P_S2_;}//默认断点为第一条命令

    virtual bool P(semaphore &s);//消费者P操作
    virtual WAITINGREASON V(semaphore &s);//消费者V操作
    void Get(deque<int>& Buffer);//消费者取得产品
    void Consume();//消费者消费产品
};

bool ProcessConsumer::P(semaphore &s) {//消费者P操作
    if (pcb.status == BLOCK)//如果是从阻塞中唤醒
        pcb.status == READY;
    else
        s--;//信号量减一
    cout << "Consumer P(S2) : " ;
    if (s<0)//信号量小于零则阻塞
    {
        cout<<"Block"<<endl;
        pcb.status=STATUS::BLOCK;
        pcb.waitingReason=WAITINGREASON::S2;
        return true;
    }
    else
    {
        cout<<"Continue"<<endl;
        pcb.status=STATUS::READY;
        pcb.waitingReason=WAITINGREASON::None;
        return false;
    }
}

WAITINGREASON ProcessConsumer::V(semaphore &s) {//消费者V操作 返回唤醒S1
    s++;//信号量加一
    cout << "Consumer V(S1) : " ;
    if (s<=0)//信号量小于1则需要唤醒
    {
        cout<<"wake up"<<endl;
        return S1;
    }
    else
    {
        cout<<"continue"<<endl;
        return None;
    }
}

void ProcessConsumer::Get(deque<int> &Buffer) {//消费者取得产品
    Product = Buffer[0];//取出缓冲区第一个产品
    Buffer.pop_front();//弹出
    cout<<"Consumer Get: "<<Product<<endl;
}

void ProcessConsumer::Consume() {//消费者消费产品
    cout<<"Consumer Consume: "<<Product<<endl;
}

#endif //PROJECT5_PRODUCERCONSUMER_H
