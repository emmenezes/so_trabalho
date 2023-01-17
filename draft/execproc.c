#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define IPC_ID 2353

struct msg
{
    int id;
    char file_name[30];
};

int main(int argc, char *argv[])
{
    int idfila;
    char *p;
    long arg = 0;
    struct msg msg_env;

    if (argc == 3)
    {
        arg = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
        {
            printf("Argumento invalido, insera apenas o numero do processo que deseja encerrar\n");
            return 1;
        }
        msg_env.id = arg + 100;
        strcpy(msg_env.file_name, argv[2]);
    }
    else
    {
        printf("Quantidade de argumentos invalida, insira apenas o número do processo que deseja encerrar\n");
        return 1;
    }

    if ((idfila = msgget(IPC_ID, 0x124)) < 0)
    {
        printf("erro na obtenção da fila da fila\n");
    }
    msgsnd(idfila, &msg_env, sizeof(msg_env), 0);
}