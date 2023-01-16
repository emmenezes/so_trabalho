#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

struct msg
{
    int id;
    char file_name[30];
};

int main(int argc, char *argv[])
{
    int idfila, LEN = 10;
    long arg = 0;
    char line[LEN];
    char *p;
    struct msg msg_env;

    if (argc == 2)
    {
        arg = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
        {
            printf("Argumento invalido, insera apenas o numero do processo que deseja encerrar\n");
            return 1; 
        }
        msg_env.id = arg;
        strcpy(msg_env.file_name, "");
    }
    else
    {
        printf("Quantidade de argumentos invalida, insira apenas o número do processo que deseja encerrar\n");
        return 1;
    }

    if ((idfila = msgget(0x0323, 0x124)) < 0)
    {
        printf("erro na obtenção da fila da fila\n");
    }
    msgsnd(idfila, &msg_env, sizeof(msg_env), 0);

    FILE *cmd = popen("pidof ex10", "r");
    fgets(line, LEN, cmd);
    pid_t pid = strtoul(line, NULL, 10);
    kill(pid, SIGUSR2);
    pclose(cmd);
}