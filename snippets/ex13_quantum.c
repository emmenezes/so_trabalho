#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid, alert = 0;

void quantum(){
    printf("para paro\n");
    kill(pid, SIGSTOP);
    alert = 1;
}


int main()
{
    signal(SIGALRM, quantum);

    int estado = 0;
    if ((pid = fork()) < 0) {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0) {
        kill(getpid(), SIGSTOP);
        execlp("./30sec", "30sec", (char * ) 0);
    } else {
        alarm(4);
        kill(pid, SIGCONT);
        if (alert || wait(&estado)) {
            printf("interrompeu\n");
            kill(pid, SIGKILL);
        } else {
            printf("acabou\n");
        }
    }
    printf("cabou mesmo\n");
    return 0;
}