#include<stdio.h>

char arr[] = "hello,world!\n";

int main()
{
	arr[3] = 'a';
	printf("%s %lu\n",arr,sizeof("aaaaaaaaaaaaaaaaaaaa"));
	return 0;
}
