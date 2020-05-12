//
// Created by dzy on 2020/5/8.
//
#include "ProducerConsumer.h"
#ifndef PROJECT5_CPU_H
#define PROJECT5_CPU_H
typedef int semaphore;

const int BufferSize = 10;//缓冲区大小
const int EndNum = 40;//结束调用次数

class CPU{//负责运行进程，修改信号量，缓冲区和进程状态。
private:
semaphore s1;//信号量1
semaphore s2;//信号量2
deque<int> Buffer;//缓冲区

public:
    void Execute(Process * process,bool & block,WAITINGREASON & wake);//执行 阻塞和唤醒通知操作系统
    CPU():s1(BufferSize),s2(0){}
};

void CPU::Execute(Process * process,bool & OutBlock,WAITINGREASON & OutWake) {//执行 阻塞和唤醒通知操作系统
    if (process== nullptr)
        return;

    cout<<"Process :"<<process->pcb.name<<" ";
    process->count++;//调用次数加一

    ProcessConsumer* processConsumer;
    ProcessProducer* processProducer;

    bool block = false;//是否阻塞
    WAITINGREASON wake = None;//唤醒信号量

    switch (process->pcb.Breakpoint)//从断点开始执行 模拟取指令和运行
    {
        case ProcessProducer::PRODUCE:{
            processProducer = (ProcessProducer *) process;
            processProducer->Produce();
            break;
        }
        case ProcessProducer::P_S1_:{
            processProducer = (ProcessProducer *) process;
            block = processProducer->P(s1);
            break;
        }
        case ProcessProducer::PUT:{
            processProducer = (ProcessProducer *) process;
            processProducer->Put(Buffer);
            break;
        }
        case ProcessProducer::V_S2_:{
            processProducer = (ProcessProducer *) process;
            wake = processProducer->V(s2);
            break;
        }
        case ProcessProducer::GO_TO_0:{
            OutBlock = false;
            OutWake = None;
            cout<<"Producer Go To 0."<<endl;
            process->pcb.Breakpoint=ProcessProducer::PRODUCE;
            if (process->count>=EndNum)
                process->pcb.status=STATUS::END;
            return;
        }
        case ProcessConsumer::P_S2_:{
            processConsumer = (ProcessConsumer*) process;
            block = processConsumer->P(s2);
            break;
        }
        case ProcessConsumer::GET:{
            processConsumer = (ProcessConsumer*) process;
            processConsumer->Get(Buffer);
            break;
        }
        case ProcessConsumer::V_S1_:{
            processConsumer = (ProcessConsumer*) process;
            wake = processConsumer->V(s1);
            break;
        }
        case ProcessConsumer::CONSUME:{
            processConsumer = (ProcessConsumer*) process;
            processConsumer->Consume();
            break;
        }
        case ProcessConsumer::GO_TO_0:{
            OutBlock = false;
            OutWake = None;
            cout<<"Consumer Go To 0."<<endl;
            process->pcb.Breakpoint=ProcessConsumer::P_S2_;
            return;
        }
    }
    process->pcb.Breakpoint++;//指令移往下一条
    //向操作系统返回信号量
    OutBlock=block;
    OutWake=wake;
}

#endif //PROJECT5_CPU_H
