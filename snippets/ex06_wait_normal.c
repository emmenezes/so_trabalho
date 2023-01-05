#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int main()
{
    int pid;
    pid_t estado;

    if ((pid = fork()) < 0)
    {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0)
    {
        printf("criou filho\n");
        execlp("./ex04", "ex04", (char *)0);
    }
    else
    {
        printf("estou esperando no pai\n");
        estado = wait(NULL);
        printf("estado %d\n", estado);
    }

    return 0;
}