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
    int MaxFreeNum;//最大的连续空闲区大小
    int MaxFreeAddr;//最大的连续空闲区首地址
    int WorkNum;//递增标志 同一次分配的块具有相同的标志
public:
    Disk(){memset(Memory,0, sizeof(Memory)/ sizeof(int));}
    void Initial();//磁盘初始化
    void UpdateMaxFree();//更新最大连续块
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
    MaxFreeAddr = 0;
    MaxFreeNum = TotalRecordsNum;
    WorkNum=0;
}

void Disk::UpdateMaxFree()//更新最大连续块
{
    if (FreeNum==0)//无空间
    {
        MaxFreeNum=0;
        MaxFreeAddr=0;
    }
    else
    {
        for (int i = MaxFreeAddr; i < TotalRecordsNum; i++)//检查后面有没有更大的连续空间
        {
            int sum = 0;//连续空间长度
            for (int j = i; j < TotalRecordsNum; j++) {
                if (Memory[j] == 0 && j != TotalRecordsNum - 1)//计算连续空间长度
                {
                    sum++;
                } else//遍历到占用块
                {
                    if (j == TotalRecordsNum - 1) sum++;//计入最后一块
                    if (sum > MaxFreeNum)//检查是否大于最大值
                    {
                        MaxFreeNum = sum;
                        MaxFreeAddr = i;
                        break;
                    }
                    i = j + 1;
                    sum = 0;//清零计数
                }
            }
        }
        for (int i = 0; i < MaxFreeAddr + MaxFreeNum; i++)//检查前面是否有更大的空间
        {
            int sum = 0;//连续空间长度
            for (int j = i; j < MaxFreeAddr + MaxFreeNum; j++) {
                if (Memory[j] == 0 && j != MaxFreeAddr + MaxFreeNum - 1)//计算连续空间长度
                {
                    sum++;
                } else//遍历到占用块
                {
                    if (j == MaxFreeAddr + MaxFreeNum - 1) sum++;//计入最后一块
                    if (sum > MaxFreeNum)//检查是否大于最大值
                    {
                        MaxFreeNum = sum;
                        MaxFreeAddr = i;
                        break;
                    }
                    i = j + 1;
                    sum = 0;//清零计数
                }
            }
        }
    }
}

void Disk::ShowAll()//打印磁盘状态
{
    colour(BLUE);cout<<"Free Memory: "<<FreeNum<<" MaxFreeNum:"<<MaxFreeNum<<" MaxFreeAddr: "<<MaxFreeAddr<<endl;
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
        WorkNum++;//标志加一
        if (blockNum<=MaxFreeNum)//申请的空间比最大连续空间小 申请连续空间
        {
            for (int i=MaxFreeAddr;i<MaxFreeAddr+blockNum;i++)
            {
                Memory[i] = WorkNum;//更新占用状态
            }
            FreeNum -= blockNum;//更新剩余空间
            //更新最大连续空间
            MaxFreeAddr += blockNum;
            MaxFreeNum -= blockNum;
            UpdateMaxFree();
        }
        else//申请的空间比最大连续空间小 则申请不连续的空间
        {
            //先将最大连续空间分配完
            for (int i=MaxFreeAddr;i<MaxFreeAddr+MaxFreeNum;i++)
                Memory[i]=WorkNum;
            blockNum -= MaxFreeNum;
            MaxFreeAddr = 0;
            MaxFreeNum = 0;
            //分配零碎空间
            int i = 0;
            while(blockNum>0)
            {
                if (Memory[i]==0)
                {
                    blockNum--;
                    Memory[i]=WorkNum;
                }
                i++;
            }
            UpdateMaxFree();//更新最大连续空间
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
    UpdateMaxFree();//更新最大块
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
