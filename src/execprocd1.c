#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h> 
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define KEY 25696

void usr1(){
    printf("1");
}

void usr2(){
    printf("2");
}

void main(){
    /* pid_t 
        - dentro do processo filho, o pid tem valor 0
        - dentro do processo pai, o pid tem o valor do processo filho
        - a fork() retorna um valor negativo, caso tenha erro.
    */
    pid_t pid; 
    int status; //estado do processo filho    
    int idFila;

    struct sigaction sa;
    //sa.sa_handler = &SigHandler;
    
    if( (idFila = msgget(KEY, IPC_CREAT | 0x1FF)) < 0){
        printf("Erro ao criar fila\n");
        exit(1);
    }
    if((pid = fork()) < 0 ){ // se houver erro, avisa o erro e termina execucao
        printf("Erro no fork!\n");
        exit(1);
    }
    
    if(pid == 0){// filho
        printf("Sou o processo filho.\nPid Filho: %d\n", getpid());
           
        signal(SIGUSR1, usr1);
        signal(SIGUSR2, usr2);
        kill(getpid(), SIGUSR1);
        kill(getpid(), SIGUSR2);
    }
    
    else{ // pai
        printf("Sou o processo pai.\nPid Pai: %d\n", getppid());

        sleep(30);      // para mostrar o processo zombie
        wait(&status);  // esperando o filho morrer

        //printf("Id: %d\nTempo total de execução: %d\nTrocas de contexto: %d\n\n", pid, exec, troca_contexto)
        exit(0); //morreu
    }


}
