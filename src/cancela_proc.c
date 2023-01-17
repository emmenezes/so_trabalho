#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
   if(argc < 2)
   {
    printf("Erro, qtd agurmentos.");
    exit(5);
   }
   kill(atoi(argv[1]), SIGKILL);
}