#include <windows.h>
#include "OS.h"
using namespace std;
enum COLOR{BLUE=1,RED=4,GREEN=10,PURPLE=5,PINK=12,YELLOW=6};
void colour(int x)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,x);
}
int main()
{
    OS os;
    os.Initial();
    os.Run();
    return 0;
}
