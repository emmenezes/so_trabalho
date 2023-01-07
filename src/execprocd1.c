#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h> 
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHAVE 000


void cria_proc_filho(){
    /* pid_t 
        - dentro do processo filho, o pid tem valor 0
        - dentro do processo pai, o pid tem o valor do processo filho
        - a fork() retorna um valor negativo, caso tenha erro.
    */
    pid_t pid; 
    
 
    if((pid = fork()) < 0 ){ // se houver erro, avisa o erro e termina execucao
        perror("Erro fork!");
        exit(1);
    }
    
    if(pid == 0) // filho
        printf("Sou o processo filho.\nPid Filho: %d\n", getpid());
    
    else{ // pai
        printf("Sou o processo pai.\nPid Pai: %d\n", getpid());
    }
}

void wait(){
    pid_t pid;

    printf("Pid Pai: %d\n", getpid());

    if((pid = fork()) != 0){
        printf("Processo pai.\nPid filho %d\n", pid);
        wait(NULL);
        printf("Processo filho morreu!\n");
        /* aqui sao as impressoes(id, tempo de execucao, trocas de contexto)*/
    }
    else{
        sleep(2); // filho espera para executar
    //    printf("Pid Filho: %d\n", getpid());
    }
}
//pid_t wait(int *status){

