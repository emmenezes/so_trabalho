/**
 * @file ex19_fork.c
 * @author Emme (menezes.ecd@gmail.com)
 * @brief Testa retorno da função fork
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;
    if ((pid = fork()) < 0) {
        printf("erro no fork\n");
        exit(1);
    }
    printf("%d\n", pid);
    if (pid == 0) {
        printf("sou o processo filho\n");
    } else {
        printf("sou o processo pai\n");
    }
    return 0;
}