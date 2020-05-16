#include <iostream>
#include <windows.h>
#include <deque>
using namespace std;

enum COLOR{BLUE=1,RED=4,GREEN=10,PURPLE=5,PINK=12,YELLOW=6,WHITE=7};
void colour(int x)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,x);
}


//磁盘相关常量
const int CylinderNum = 8;//柱面数
const int MagneticTrackNum = 2;//磁道数
const int PhysicalRecordsNum = 4;//物理记录数
const int TotalRecordsNum = CylinderNum * MagneticTrackNum * PhysicalRecordsNum;//总数

class Disk{//磁盘
    int Memory[TotalRecordsNum];//记录磁盘空间使用情况 0为空闲 非0为占用标志
    int FreeNum;//总空闲大小
    int WorkNum;//递增标志 同一次分配的块具有相同的标志
public:
    Disk(){memset(Memory,0, sizeof(Memory)/ sizeof(int));}
    void Initial();//磁盘初始化
    void ShowAll();//打印磁盘状态
    void ShowByNum(int n);//打印对应标志位的地址
    int Allocate(int blockNum);//分配函数 返回标志号 0代表分配失败
    bool Recycle(int workNum);//回收函数，返回是否回收成功
};

void Disk::Initial()//磁盘初始化
{
    for (int i=0;i<TotalRecordsNum ;i++)
        Memory[i] = 0;
    FreeNum = TotalRecordsNum;
    WorkNum=0;
}

void Disk::ShowAll()//打印磁盘状态
{
    colour(BLUE);cout<<"Free Memory: "<<FreeNum<<endl;
    for (int i=0;i<CylinderNum;i++)//遍历每个柱面
    {
        for (int j=0;j<MagneticTrackNum*PhysicalRecordsNum;j++)
        {
            cout<<" ";
            if (Memory[i*CylinderNum+j] == 0)//空闲
            {
                colour(GREEN);cout<<"0";
            }
            else//占用
            {
                colour(RED);cout<<"1";
            }
        }
        cout<<endl;
    }
}

void Disk::ShowByNum(int n)//打印对应标志位的地址
{
    colour(WHITE);
    cout << "Num:" << n << endl;
    colour(PINK);
    cout << "CylinderNum" << " MagneticTrackNum"
         << " PhysicalRecordsNum" << endl;
    for (int i=0;i<TotalRecordsNum;i++)
    {
        if (Memory[i]==n)//找到该标志
        {
            cout << "\t" << i / 8 << "\t" << i / 4 << "\t" << i % 4 << endl;
        }
    }
    cout<<endl;
}

int Disk::Allocate(int blockNum)//分配函数 返回标志号 0代表分配失败
{
    if (blockNum>FreeNum)//空间不足
    {
        return 0;
    }
    else
    {
        FreeNum-=blockNum;
        WorkNum++;//标志加一
        for (int i=0;i<TotalRecordsNum;i++)
        {
            if (Memory[i]==0)
            {
                Memory[i]=WorkNum;
                blockNum--;
            }
            if (blockNum==0) break;
        }
        return WorkNum;
    }
}

bool Disk::Recycle(int workNum)//回收函数，返回是否回收成功
{
    if (workNum<=0)
        return false;
    bool ReturnFlag = false;//返回值
    ShowByNum(workNum);
    for (int i=0;i<TotalRecordsNum;i++)
    {
        if (Memory[i]==workNum)
        {
            FreeNum++;//空闲块加一
            ReturnFlag=true;
            Memory[i]=0;//回收空间
        }
    }
    return ReturnFlag;
}

class OS{
    Disk disk;//硬盘
public:
    void Initial()//初始化
    {
        disk.Initial();
    }
    void Run()//运行
    {
        disk.ShowAll();
        int input;
        while (true)
        {
            colour(PURPLE);cout<<"enter 1 to allocate, 2 to recycle: ";
            cin>>input;
            if (input == 1)
            {
                    colour(YELLOW);cout<<"enter the num:";
                    int blockNum;
                    cin>>blockNum;
                    int WorkNum=disk.Allocate(blockNum);
                    if (WorkNum == 0)//申请失败
                    {
                        colour(RED);cout<<"No Enough Space."<<endl;
                    }
                    else
                    {
                        disk.ShowByNum(WorkNum);
                    }
            }
            else if (input == 2)
            {
                int workNum;
                cout<<"enter the num:";
                cin>>workNum;
                if (disk.Recycle(workNum)== false)//回收失败
                {
                    colour(RED);cout << "Error Num." << endl;
                }
            }
            else continue;
            disk.ShowAll();
        }
    }
};
int main() {
    OS os;
    os.Initial();
    os.Run();
    return 0;
}
