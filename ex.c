#include<stdio.h>
#include<string.h>


char call[20] = {'a','c','b'};

int main()
{
	memset(call, 0, sizeof(char));
	if(strlen(call) == 0){
		printf("hah\n");
	}
	return 0;
}
