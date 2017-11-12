#include<stdio.h>

int print(int s)
{
    if(s > 10)
        printf("%d\n",s);
    return 0;
}

int main()
{
    int (*fp)(int);
    int a = 11;
    fp = print;
    (*fp)(a);
    print(a+1);
    return 0;
}
