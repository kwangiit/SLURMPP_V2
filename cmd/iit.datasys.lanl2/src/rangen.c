#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void main()
{
    srand(time(NULL));
    int i = 0;
    int num = 0;
    for (i = 0; i < 20; i++)
    {
	num = rand() % 50 + 25;
	printf("%d\n", num);
    }
}
