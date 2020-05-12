#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>
using namespace std;
int main()
{
    pid_t pid1 = fork();
    pid_t pid2;
    if (pid1<0) 
        cout<<"Error"<<endl;
    else if (pid1==0)
        cout<<"B,ppid:"<<getppid()<<",pid:"<<getpid()<<endl;
    else 
    {
    cout<<"A,pid:"<<getpid()<<endl;
    pid2=fork();
    if (pid2<0) cout<<"Error"<<endl;
    else if (pid2==0)
        cout<<"C,ppid:"<<getppid()<<",pid:"<<getpid()<<endl;
    }
    waitpid(pid1,0,0);
    waitpid(pid2,0,0);
    return 0;
}