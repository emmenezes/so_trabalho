#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define IPC_ID 2353

struct msg {
    int id;
    char file_name[30];
};

int main() {
    int idfila, LEN=10;
    char line[LEN];
    struct msg msg_env;

    if ((idfila = msgget(IPC_ID,0x124)) < 0) {
        printf("erro na obtenção da fila da fila\n");
    }
    msg_env.id = -1;
    strcpy(msg_env.file_name, "");
    msgsnd(idfila, &msg_env, sizeof(msg_env), 0);

    FILE *cmd = popen("pidof ex2", "r");
    fgets(line, LEN, cmd);
    pid_t pid = strtoul(line, NULL, 10);
    kill(pid, SIGUSR2);
    pclose(cmd);
}