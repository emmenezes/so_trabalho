/**
 * @file ot01_varios_forks.c
 * @author Emme (menezes.ecd@gmail.com)
 * @brief Cria uma quantidade X de processos filho
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

int main() {
    int pid;
    
    for (int i = 0; i < 3; i++){
        pid = fork();
        if (pid == 0)
            break;
    }

    printf("Sou um processo!\n");
    return 0;
}