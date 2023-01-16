#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

struct msg {
    int id;
};

int main() {
    int idfila, LEN=10;
    char line[LEN];
    struct msg msg_env;

    if ((idfila = msgget(0x0323,0x124)) < 0) {
        printf("erro na obtenção da fila da fila\n");
    }
    msg_env.id = -1;
    msgsnd(idfila, &msg_env, sizeof(msg_env), IPC_NOWAIT);

    FILE *cmd = popen("pidof ex10", "r");
    fgets(line, LEN, cmd);
    pid_t pid = strtoul(line, NULL, 10);
    kill(pid, SIGUSR2);
    pclose(cmd);
}