/**
 * @file s31_pipe.c
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
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid,    /* pid od processo filho */
        fd[2],  /* descritores do pipe */
        estado; /* estado do processo filho */
    char mensagem[30];
    if (pipe(fd) < 0)
    {
        printf("erro na criacao do pipe\n");
        exit(1);
    }
    if ((pid = fork()) < 0)
    {
        printf("erro na criacao do processo\n");
        exit(1);
    }
    if (pid == 0)
    {
        if (read(fd[0], mensagem, 30) < 0)
            printf("erro na leitura\n");
        else
            printf("valor lido = %s\n", mensagem);
        exit(0);
    }
    strcpy(mensagem, "teste do envio no pipe");
    sleep(20);
    if (write(fd[1], mensagem, sizeof(mensagem)) < 0)
        printf("erro na escrita\n");
    wait(&estado);
    exit(0);
}