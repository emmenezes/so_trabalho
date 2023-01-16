
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
#include <string.h>

#define KEY 0x25696

void usr1(){
    printf("Recebi o SIGUSR1\n");
}

void usr2(){
    printf("Recebi o SIGUSR2\n");
}

void main(){
    /* pid_t 
        - dentro do processo filho, o pid tem valor 0
        - dentro do processo pai, o pid tem o valor do processo filho
        - a fork() retorna um valor negativo, caso tenha erro.
    */
    pid_t pid; 
    int status; //estado do processo filho    
    int idFila; // identificador da fila

    struct mensagem{
        long pid;
        char msg[30];
    };
    
    struct mensagem msg_env, msg_rec;
    
    if( (idFila = msgget(KEY, IPC_CREAT | IPC_EXCL | 0x1FF)) < 0){ //criando fila com ipc exclusivo
        printf("Erro ao criar fila\n");
        exit(1);
    }
    if((pid = fork()) < 0 ){ // se houver erro, avisa o erro e termina execucao
        printf("Erro no fork!\n");
        exit(1);
    }
    
    if(pid == 0){// filho
        printf("Sou o processo filho.\n");
        msg_env.pid = getpid();
        printf("Pid Filho: %d\n", getpid());
        sleep(30);
        /*envia o idFila, o tamanho de mensagem enviada, tira o long e eh blocante */
        msgsnd(idFila, &msg_env, sizeof(msg_env)-sizeof(long), 0); 

        /* quando receber os sinais SIGUSR desvia para as funções*/
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

    /* implementar o sigalrm para o quantum*/
}
