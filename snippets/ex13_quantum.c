#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid, alert = 0, estado;

void quantum()
{
    if (waitpid(pid, &estado, WNOHANG)) {
        printf("ta morto\n");
        alert = 1;
    } else {
        printf("ta vivo\n");
        kill(pid, SIGSTOP);
        alert = -1;
    }
    alarm(0);
}

int main()
{
    signal(SIGALRM, quantum);

    if ((pid = fork()) < 0)
    {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0) // processo filho
    {
        kill(getpid(), SIGSTOP);
        execlp("./30sec", "30sec", (char *)0);
    }
    else // processo pai
    {
        printf("pid %d\n", pid);
        alert = -1;
        kill(pid, SIGSTOP);
        while(alert != 1) {
            if (alert != 0) {
                kill(pid, SIGCONT);
                alarm(4);
                alert = 0;
            }
        }
    }
    printf("cabou mesmo\n");
    return 0;
}