#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid, ret = 0, estado;

int main()
{
    int counter = 0;
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
        while (!ret) {
            kill(pid, SIGSTOP);
            counter++;
            kill(pid, SIGCONT);
            sleep(4);
            ret = waitpid(pid, &estado, WNOHANG);
        }
    }
    printf("cabou mesmo em %d trocas\n", counter);
    return 0;
}