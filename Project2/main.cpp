#include <iostream>
#include <windows.h>
using namespace std;

enum COLOR{BLUE=1,RED=4,GREEN=10,PURPLE=5,PINK=12,YELLOW=6};
void colour(int x)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,x);
}

const int MaxMemory = 128;

class Node{//空闲分区链的节点
    int address;//空闲分区起始地址
    int memory;//占用空间
public:
    Node * next;//下一个节点
    Node():address(0),memory(0),next(nullptr){}
    Node(int ad, int mem):address(ad),memory(mem),next(nullptr){}
    int GetMem(){return memory;}//返回剩余空间
    int GetAddr(){return address;}//返回地址
    void Allocate(int mem){address+=mem;memory-=mem;}//分配mem大小的空间
    void Merge(int mem){memory+=mem;}//合并mem大小的空间
    void Show(int n = 0)
    {
        colour(BLUE);cout<<n<<"\t"<<address<<"\t"<<memory<<"\t"<<endl;
    }
    void Swap(Node * n)
    {
        swap(address,n->address);
        swap(memory,n->memory);
    }
};

class LinkList{//分区链
    Node * head;//链表头结点
public:
    LinkList(){head=new Node(-1,-1);}
    void AddNode(int ad,int mem);//在尾部添加节点
    Node * FindMem(int mem);//找到第一个空间大于mem的节点
    Node * FindNum(int num);//返回第num个节点
    Node * FindAddr(int addr);//返回地址为addr的节点
    void Delete(Node * Dnode);//删除一个节点
    void Show();//打印信息
    void Sort();//从低地址向高地址排序 地址交换 非值交换
    void Merge(Node * node);//用于合并空闲分区前后的空闲分区
};

void LinkList::AddNode(int ad, int mem)
{
    Node * node=head;
    while (node->next!= nullptr)
        node=node->next;//找到最后一个节点
    node->next=new Node(ad,mem);
}

Node *LinkList::FindMem(int mem)     {
    Node * node = head->next;
    while (node!= nullptr)
    {
        if (node->GetMem()>=mem)//如果空间大于要求
        {
            return node;//返回该节点
        }
        node=node->next;
    }
    return nullptr;//没找到返回空指针
}

Node *LinkList::FindNum(int num) {
    int i=1;
    Node * node = head->next;
    while (node!= nullptr)
    {
        if (i==num) return node;
        node=node->next;
        i++;
    }
    return nullptr;
}

Node *LinkList::FindAddr(int addr) {
    Node * node = head->next;
    while (node!= nullptr)
    {
        if (node->GetAddr()==addr) return node;
        node=node->next;
    }
    return nullptr;
}

void LinkList::Delete(Node *Dnode) {
    Node* node= head;
    while (node!= nullptr)
    {
        if (node->next==Dnode)
        {
            node->next = Dnode->next;
            delete Dnode;
        }
        node=node->next;
    }
}

void LinkList::Show()     {
    int i=0;//计数项
    Node * node=head->next;
    colour(GREEN);cout<<"Num"<<"\t"<<"Address"<<"\t"<<"Size"<<endl;
    while (node != nullptr)//遍历链表
    {
        i++;
        node->Show(i);
        node=node->next;
    }
}

void LinkList::Sort() {
    Node * One = head->next;//递增节点1
    Node * Two;//递增节点2
    Node * MinNode;//每轮内层循环中地址最小的节点
    while(One!= nullptr)
    {
        Two = One;
        MinNode = One;
        while(Two!= nullptr)
        {
            if (Two->GetAddr()<MinNode->GetAddr())
            {
                MinNode=Two;
            }
            Two=Two->next;
        }
        if (MinNode!=One)//找到了错位最小值
        {
            MinNode->Swap(One);//交换节点值
        }
        One=One->next;
    }
}

void LinkList::Merge(Node *node) {
    if (node== nullptr) return;
    Node*FrontNode=head;//前节点
    while(FrontNode->next!=node && FrontNode!= nullptr)//找到node前面的节点
        FrontNode=FrontNode->next;
    Node*BackNode=node->next;//后节点
    if (BackNode!=nullptr)//尝试合并后面与当前的节点
    {
        if (node->GetAddr()+node->GetMem()==BackNode->GetAddr())//满足合并条件
        {
            node->Merge(BackNode->GetMem());//合并节点
            Delete(BackNode);//删除后节点
        }
    }
    if (FrontNode->GetAddr()+FrontNode->GetMem()==node->GetAddr())//满足合并前节点与当前节点
    {
        FrontNode->Merge(node->GetMem());//合并节点
        Delete(node);//删除当前节点
    }
}

class OS{
    LinkList FreeLinkList;//空闲分区链
    LinkList AllocLinkList;//存储分配的分区
public:
    void Initial(){FreeLinkList.AddNode(0, MaxMemory);}//初始化
    OS(){}
    void Input();//获取输入
    bool Allocate();//分配
    bool Recycle();//回收
};

void OS::Input() {
    bool cycle = true;//循环变量
    while (cycle)
    {
        colour(PINK);cout<<"Free:"<<endl;
        FreeLinkList.Show();//打印当前空闲分区信息
        colour(PINK);cout<<"Alloc:"<<endl;
        AllocLinkList.Show();//打印已分配信息
        int input;
        colour(PURPLE);cout<<"Enter 1 to allocate, 2 to recycle, others to quit:";
        cin>>input;
        switch (input)
        {
            case 1:Allocate();break;
            case 2:Recycle();break;
            default:cycle=false;break;
        }
    }
}

bool OS::Allocate() {
    colour(YELLOW);cout<<"Please enter the memory:";
    int mem;cin>>mem;
    if (mem<=0)//错误输入
    {
        colour(RED);cout<<"Invalid Input."<<endl;
        return false;
    }
    else
    {
        Node * AllocNode = FreeLinkList.FindMem(mem);//找到最前面的满足要求的空闲节点
        if(AllocNode!= nullptr)
        {
            AllocLinkList.AddNode(AllocNode->GetAddr(),mem);
            AllocLinkList.Sort();
            AllocNode->Allocate(mem);
            if (AllocNode->GetMem()==0)//该空闲节点被分配完
                FreeLinkList.Delete(AllocNode);//移除该空闲节点
        }
        else//分配失败
        {
            colour(RED);cout<<endl<<"No enough mem."<<endl<<endl;
            return false;
        }
    }
    return true;
}

bool OS::Recycle() {
    colour(YELLOW);cout<<"Please enter the num to recycle:";
    int num;cin>>num;
    if (num<=0)//错误输入
    {
        colour(RED);cout<<"Invalid Input."<<endl;
        return false;
    }
    else
    {
        Node * node = AllocLinkList.FindNum(num);
        if (node == nullptr)//没找到该节点
        {
            colour(RED);cout<<"Not Found."<<endl;
            return false;
        }
        else//找到该节点
        {
            FreeLinkList.AddNode(node->GetAddr(),node->GetMem());//添加入空闲区
            FreeLinkList.Sort();
            FreeLinkList.Merge(FreeLinkList.FindAddr(node->GetAddr()));
            AllocLinkList.Delete(node);//从分配区中移除
        }
    }
    return true;
}

int main() {
    OS os;
    os.Initial();
    os.Input();
    return 0;
}
