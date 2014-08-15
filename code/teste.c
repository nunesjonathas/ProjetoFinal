#include <stdio.h>

int main(void)
{
	for (int i = 0; i < 3; ++i)
	{
		printf("%d\t %d\n", i%3,(i+1)%3);
   
	}

	return 0;
}