#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int pid;
    int input = 0;

    while (1)
    {
        scanf("%d", &input);
        if (input)
        {
            if ((pid = fork()) < 0)
            {
                printf("erro no fork\n");
                exit(1);
            }
            if (pid == 0)
            {
                printf("criou filho\n");
                kill(getpid(), SIGTSTP);
                execlp("./ex04", "ex04", (char*) 0);
            } else {
                printf("estou esperando no pai\n");
                scanf("%d", &input);
                kill(pid, SIGCONT);
            }
        }
        else
        {
            break;
        }
    }
    return 0;
}