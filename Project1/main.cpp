#include <iostream>
#include <deque>
#include <algorithm>
#include <string>
#include <windows.h>
using namespace std;
enum COLOR{BLUE=1,RED=4,GREEN=10,PURPLE=5,PINK=12,YELLOW=6};
void colour(int x)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,x);
}

const int MaxPCBNum = 3;//进程数

class PCB{
    string name;//进程名
    // PCB * nextPCB;//下一个进程PCB的地址
    int time;//要求运行时间
    int priority;//优先数
    bool status;//状态 0为完成 1为就绪
    //***其他内容
public:
    PCB(){name="none";time=0;priority=0;status= false;}//默认构造函数
    PCB(string n,int t,int p,bool s):name(n),time(t),priority(p),status(s){}//构造函数
    int GetPriority(){return priority;}//返回优先数
    bool GetStatus(){return status;}//返回状态
    bool operator<(PCB pcb){return priority>pcb.priority;}//重载小于以使用sort 因为从大到小排序 所以返回大与
    bool operator==(PCB pcb){return priority==pcb.priority;}//重载等于以使用sort
    bool Run()//运行 返回是否成功运行
    {
        if (time<1||!status)//参数错误
            return false;
        else
        {
            time--;
            priority--;
            if (time==0) status = false;//更新状态
            cout<<name<<" is running."<<endl;
            return true;
        }
    }
    bool Input()//输入PCB信息 返回是否成功创建
    {
        colour(BLUE);
        string n;//name
        int t,p;//time priority
        cout<<"Please enter the name of Process:\n";
        cin >> n;
        name = n;
        cout<<"Please enter the time:\n";
        cin >> t;
        cout<<"Please enter the priority:\n";
        cin>>p;
        if (t<0||p<0)//非法输入
        {
            colour(RED); cout<<"Invalid Input."<<endl;
            return false;//创建失败
        }
        else
        {
            priority=p;
            time=t;
            status=true;
            cout<<"The Process you created:"<<endl;//打印刚输入的进程信息
            colour(YELLOW); cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            Show(PURPLE);
            return true;//创建成功
        }
    }
    void Show(int color = GREEN)//打印进程信息
    {
        colour(color);
        cout << name << "  \t" << time << "  \t" << priority << "\t\t" << (status==false?'E':'R') << endl;
    }
};

class OS{
    deque<PCB> ReadyDeque;//就绪队列
    deque<PCB> EndDeque;//完成队列
public:
    OS(){}
    void Initial()//初始化
    {

    }
    void Input()//输入信息
    {
        for (int i=0;i<MaxPCBNum;i++)
        {
            PCB pcb;//创建进程
            colour(PINK);cout<<"Process "<< i+1 <<endl;
            if (pcb.Input())//如果创建合法则添加入就绪队列
            {
                ReadyDeque.push_back(pcb);
            }
            else//创建不合法
            {
                colour(RED);cout<<"Please enter again."<<endl;
                i--;
            }
        }
        sort(ReadyDeque.begin(), ReadyDeque.end());//创建完后按优先数从大到小的顺序排列
    }
    void Show(bool ready)//打印队列进程信息 ready为true打印就绪队列 false打印完成队列
    {
        if (ready) //就绪队列
        {
            colour(GREEN);cout << "Ready:" << endl;
            colour(YELLOW);cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            for (auto i = ReadyDeque.begin(); i != ReadyDeque.end(); i++)
                i->Show();
        }
        else//完成队列
        {
            colour(PINK);cout << "End:" << endl;
            colour(YELLOW);cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            for(auto i = EndDeque.begin(); i != EndDeque.end(); i++)
                i->Show();
        }

    }

    void Run()//运行就绪队列中的进程
    {
        getchar();//吞入上一个回车
        int num = 1;//记录轮数
        while (!ReadyDeque.empty())//就绪队列非空
        {
            colour(RED);cout << "Turn " << num <<": ";
            ReadyDeque.begin()->Run();//运行优先度最高的进程 此进程优先级和时间均减一
            if (ReadyDeque.begin()->GetStatus() == false)//队首进程完成则出队并添加入完成队列
            {
                EndDeque.push_back(ReadyDeque[0]);
                ReadyDeque.pop_front();
            }
            sort(ReadyDeque.begin(),ReadyDeque.end());//再次排序就绪队列
            Show(true);//打印排序后的就绪队列
            if (!EndDeque.empty())//如果完成队列非空
                Show(false);//打印完成队列
            getchar();//等待输入后继续
            num++;//轮数加一
        }
        cout<<"All end.";//全部完成
    }
};

int main() {
    OS os;
    os.Input();
    os.Show(true);
    os.Run();
    return 0;
}
