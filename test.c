#include<stdio.h>
#include<unistd.h>

int main()
{
	printf("Hello Linux\r");
	fflush(stdout);
	sleep(1);
	return 0;
}
