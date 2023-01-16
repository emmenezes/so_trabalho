#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

    clock_t start, end;
    double time;
    start = clock();

    long int i;
    // for (i = 0; i < 20000000000; i++); // 37s
    for (i = 0; i < 8000000000; i++); // 14s
    end = clock();

    time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("time = %lf\n", time);

    return 0;
}
