//
// Created by dzy on 2020/5/8.
//
#include "CPU.h"
#include <time.h>
#ifndef PROJECT5_OS_H
#define PROJECT5_OS_H

class OS{
private:
    CPU cpu;
    deque<Process*> ReadyQueue;//就绪队列
    deque<Process*> BlockQueue;//阻塞队列
public:
    OS() {}
    void Initial(){//初始化
        ProcessConsumer * processConsumer = new ProcessConsumer("c1");//创建消费者进程
        ProcessProducer * processProducer = new ProcessProducer("p1");//创建生产者进程
        //将进程添加入就绪队列
        ReadyQueue.push_back(processProducer);
        ReadyQueue.push_back(processConsumer);
    }
    void Run();//运行
    Process * Dispatch();//进程调度
    void Wake(WAITINGREASON wake);//进程唤醒
    void Block(Process* process);//进程阻塞
    void End(Process* process);//进程结束
};

Process *OS::Dispatch() {
    if (ReadyQueue.empty())//就绪队列为空
        return nullptr;
    else
    {
        int num = ReadyQueue.size();//获取就绪队列中的进程数
        srand(unsigned(time(NULL)));//随机种子
        int n = rand() % num;//随机选择一个进程
        return ReadyQueue[n];
    }
}

void OS::Wake(WAITINGREASON wake) {
    if (BlockQueue.empty())
        return;
    else
    {
        Process* process;
        auto i = BlockQueue.begin();
        for (;i!=BlockQueue.end();i++)//遍历阻塞队列
            if ((*i)->pcb.waitingReason==wake)//符合唤醒条件
                break;
        process = *i;
        BlockQueue.erase(i);//从阻塞队列中移除
        ReadyQueue.push_back(process);//添加入就绪队列
    }
}

void OS::Block(Process *process) {
    if (process== nullptr) return;
    else
    {
        BlockQueue.push_back(process);//加入阻塞队列
        auto i = ReadyQueue.begin();//在就绪列表中找到该进程
        for(;i!=ReadyQueue.end();i++)
        {
            if (*i==process)
                break;
        }
        if (i!=ReadyQueue.end())//从就绪队列中移除
            ReadyQueue.erase(i);
    }
}

void OS::End(Process *process) {
    auto i = ReadyQueue.begin();//在就绪列表中找到该进程
    for(;i!=ReadyQueue.end();i++)
    {
        if (*i==process)
            break;
    }
    if (i!=ReadyQueue.end())//从就绪队列中移除
        ReadyQueue.erase(i);
}

void OS::Run() {
    while (true)
    {
        bool block = false;//阻塞信号
        WAITINGREASON wake = None;//唤醒信号

        Process* process = Dispatch();//选择一个进程开始运行
        cpu.Execute(process,block,wake);//将CPU资源分配给进程，监听阻塞与唤醒

        if (block)//发生阻塞 权限交给操作系统
        {
            Block(process);//阻塞该进程
        }
        if (wake != None)//需要操作系统唤醒某进程
        {
            Wake(wake);
        }
        if (process->IsEnd())//结束进程
        {
            End(process);
        }
        getchar();
    }
}

#endif //PROJECT5_OS_H
