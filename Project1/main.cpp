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

const int MaxPCBNum = 3;//������

class PCB{
    string name;//������
    // PCB * nextPCB;//��һ������PCB�ĵ�ַ
    int time;//Ҫ������ʱ��
    int priority;//������
    bool status;//״̬ 0Ϊ��� 1Ϊ����
    //***��������
public:
    PCB(){name="none";time=0;priority=0;status= false;}//Ĭ�Ϲ��캯��
    PCB(string n,int t,int p,bool s):name(n),time(t),priority(p),status(s){}//���캯��
    int GetPriority(){return priority;}//����������
    bool GetStatus(){return status;}//����״̬
    bool operator<(PCB pcb){return priority>pcb.priority;}//����С����ʹ��sort ��Ϊ�Ӵ�С���� ���Է��ش���
    bool operator==(PCB pcb){return priority==pcb.priority;}//���ص�����ʹ��sort
    bool Run()//���� �����Ƿ�ɹ�����
    {
        if (time<1||!status)//��������
            return false;
        else
        {
            time--;
            priority--;
            if (time==0) status = false;//����״̬
            cout<<name<<" is running."<<endl;
            return true;
        }
    }
    bool Input()//����PCB��Ϣ �����Ƿ�ɹ�����
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
        if (t<0||p<0)//�Ƿ�����
        {
            colour(RED); cout<<"Invalid Input."<<endl;
            return false;//����ʧ��
        }
        else
        {
            priority=p;
            time=t;
            status=true;
            cout<<"The Process you created:"<<endl;//��ӡ������Ľ�����Ϣ
            colour(YELLOW); cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            Show(PURPLE);
            return true;//�����ɹ�
        }
    }
    void Show(int color = GREEN)//��ӡ������Ϣ
    {
        colour(color);
        cout << name << "  \t" << time << "  \t" << priority << "\t\t" << (status==false?'E':'R') << endl;
    }
};

class OS{
    deque<PCB> ReadyDeque;//��������
    deque<PCB> EndDeque;//��ɶ���
public:
    OS(){}
    void Initial()//��ʼ��
    {

    }
    void Input()//������Ϣ
    {
        for (int i=0;i<MaxPCBNum;i++)
        {
            PCB pcb;//��������
            colour(PINK);cout<<"Process "<< i+1 <<endl;
            if (pcb.Input())//��������Ϸ���������������
            {
                ReadyDeque.push_back(pcb);
            }
            else//�������Ϸ�
            {
                colour(RED);cout<<"Please enter again."<<endl;
                i--;
            }
        }
        sort(ReadyDeque.begin(), ReadyDeque.end());//��������������Ӵ�С��˳������
    }
    void Show(bool ready)//��ӡ���н�����Ϣ readyΪtrue��ӡ�������� false��ӡ��ɶ���
    {
        if (ready) //��������
        {
            colour(GREEN);cout << "Ready:" << endl;
            colour(YELLOW);cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            for (auto i = ReadyDeque.begin(); i != ReadyDeque.end(); i++)
                i->Show();
        }
        else//��ɶ���
        {
            colour(PINK);cout << "End:" << endl;
            colour(YELLOW);cout<<"Name"<<'\t'<<"Time"<<'\t'<<"Priority"<<'\t'<<"status"<<endl;
            for(auto i = EndDeque.begin(); i != EndDeque.end(); i++)
                i->Show();
        }

    }

    void Run()//���о��������еĽ���
    {
        getchar();//������һ���س�
        int num = 1;//��¼����
        while (!ReadyDeque.empty())//�������зǿ�
        {
            colour(RED);cout << "Turn " << num <<": ";
            ReadyDeque.begin()->Run();//�������ȶ���ߵĽ��� �˽������ȼ���ʱ�����һ
            if (ReadyDeque.begin()->GetStatus() == false)//���׽����������Ӳ��������ɶ���
            {
                EndDeque.push_back(ReadyDeque[0]);
                ReadyDeque.pop_front();
            }
            sort(ReadyDeque.begin(),ReadyDeque.end());//�ٴ������������
            Show(true);//��ӡ�����ľ�������
            if (!EndDeque.empty())//�����ɶ��зǿ�
                Show(false);//��ӡ��ɶ���
            getchar();//�ȴ���������
            num++;//������һ
        }
        cout<<"All end.";//ȫ�����
    }
};

int main() {
    OS os;
    os.Input();
    os.Show(true);
    os.Run();
    return 0;
}
