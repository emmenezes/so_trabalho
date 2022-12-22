#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    int pid, idshm, estado;
    struct shmid_ds buf;
    int *pshm;

    if ((idshm = shmget(0x1223, sizeof(int), IPC_CREAT|0x1ff)) < 0) {
        printf("erro na criacao da fila\n");
        exit(1);
    }
    if ((pid = fork()) == 0) {
        pshm = (int *) shmat(idshm, (char *) 0, 0);
        getchar();
        if (pshm == (int *) -1) {
            printf("erro no attach\n");
            exit(1);
        }
        printf("vou escrever\n");
        *pshm = 12;
        exit(0);
    }
    pshm = (int *) shmat(idshm, (char *) 0, 0);
    if (pshm == (int *) - 1) {
        printf("erro no attach\n");
        exit(1);
    }
    sleep(1);
    printf("pai - numero lido = %d\n", *pshm);
    wait(&estado);
    exit(0);
}