/**
 * @file s21_execl_pai.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-12-13
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
        execl("s21_execl_filho", "s21_execl_filho", (char*) 0);
    }
    printf("continuo o codigo do pai\n");
    sleep(20);
    printf("fim\n");
    return(0);
}