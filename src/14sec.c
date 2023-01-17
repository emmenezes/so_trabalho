#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    long int i;
    // for (i = 0; i < 20000000000; i++); // 37s
    for (i = 0; i < 8000000000; i++); // 14s

    return 0;
}
