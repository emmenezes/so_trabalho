#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MATRICULA0 170032356
#define MATRICULA1 170125696
#define MATRICULA2 170129306

int main()
{
    int pid, idfila, estado;
    struct msg 
    {
        long pid;
        char msg[100];
    };

    struct msg msg_env, msg_rec;
    if ((idfila = msgget(MATRICULA0, IPC_CREAT|0x1FF)) < 0 || 
        (idfila = msgget(MATRICULA1, IPC_CREAT|0x1FF)) < 0 || 
        (idfila = msgget(MATRICULA2, IPC_CREAT|0x1FF)) < 0)
    {
        printf("Erro na criação da fila\n");
        exit(1);
    }

    //chamada dos processos, se encontra no execprocd, onde tem o fork.
    
}