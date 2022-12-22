/**
 * @file s40_fila_msg.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    int pid, idfila, estado;
    struct msg {
        long pid;
        char msg[30];
    };

    struct msg msg_env, msg_rec;
    if ((idfila = msgget(0x1223, IPC_CREAT|0x1FF)) < 0) {
        printf("erro na criação da fila\n");
        exit(1);
    }
    pid = fork();
    if (pid == 0) {
        msg_env.pid = getpid();
        strcpy(msg_env.msg, "teste de mensagem");
        msgsnd(idfila, &msg_env, sizeof(msg_env), 0);
        exit(0);
    }
    msgrcv(idfila, &msg_rec, sizeof(msg_rec), 0, 0);
    printf("mensagem recebeida = %ld %s\n", msg_rec.pid, msg_rec.msg);
    wait(&estado);
    exit(0);

}