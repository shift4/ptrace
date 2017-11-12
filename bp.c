#include<stdio.h>

int print(int s)
{
	if(s > 10){
		printf("%d greater than 10.\n",s);
	}
	return 0;
}

int main()
{
	int (*fp)(int);
	int i = 11;
	fp = print;
	printf("1st\n");
	(*fp)(i);
	printf("2nd\n");
	print(i);
	return 0;
}
