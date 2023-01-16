#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

#define INTR_CREATE 1
#define INTR_CLOSE 2
#define INTR_EXIT 3

struct msg
{
    int id;
};

struct msg msg_rec;
struct msqid_ds info;
int idfila;
int interruption = 0;

void func_sig_create()
{
    printf("recebeu sigusr1\n");
    msgrcv(idfila, &msg_rec, sizeof(msg_rec), 0, 0);
    interruption = INTR_CREATE;
}

void func_sig_close()
{
    printf("recebeu sigusr2\n");
    msgrcv(idfila, &msg_rec, sizeof(msg_rec), 0, 0);
    if (msg_rec.id == -1)
    {
        printf("INTR_EXIT\n");
        interruption = INTR_EXIT;
    }
    else
    {
        printf("INTR_CLOSE %d\n", msg_rec.id);
        interruption = INTR_CLOSE;
    }
}

int main()
{
    if ((idfila = msgget(0x0323, IPC_CREAT | 0x1FF)) < 0)
    {
        printf("erro na criação da fila\n");
    }
    signal(SIGUSR1, func_sig_create);
    signal(SIGUSR2, func_sig_close);

    while(1) {
        if (interruption == INTR_EXIT){
            break;
        }
    }
    msgctl(idfila, IPC_RMID, &info);
    printf("fechando ex10\n");
}