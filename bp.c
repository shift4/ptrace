#include<stdio.h>
#include<unistd.h>

int print(int s)
{
    if(s % 2 == 0)
        printf("even %d\n",s);
    return 0;
}

int print2(int s)
{
    if(s % 2 == 1)
        printf("odd %d\n",s);
    return 0;
}

int main()
{
    int (*fp)(int);
    int a = 1;
	while(1){
		sleep(3);
		print(a);
		fp = print2;
		(*fp)(a);
		fp = print;
		(*fp)(a);
		a++;
	}
    return 0;
}
