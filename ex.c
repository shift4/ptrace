#include<stdlib.h>
#include<unistd.h>

void bad_foo()
{
	int *a  = (int *) malloc(40);
	a[10] = 1;
}

int main()
{
	bad_foo();
	sleep(10);
	return 0;
}
