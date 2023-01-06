/**
 * @file execprocd3.c
 * @author your name (you@domain.com)
 * @brief
 *      - Tem fila de arquivos
 *      - Recebe arquivos do exeproc
 *      - Recebe cancela_proc
 *      - Recebe termina_proc
 * @version 0.1
 * @date 2023-01-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#define QUEUE_SIZE 10
#define RANDOM_MODE 1
#define STATIC_MODE 2
#define DYNAMIC_MODE 3
#define HIGH_PRIORITY 1
#define MID_PRIORITY 2
#define LOW_PRIORITY 3
#define INTR_CREATE 1
#define INTR_CLOSE 2

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    int priority;
    char file_name[20];
} msg, *msg_rec;

typedef struct
{
    int pid;
    int id;
    int context_switches;
    time_t start_time;
    time_t end_time;
    int prev_id;
    int next_id;
    int priority;
} proc;

typedef struct
{
    int size;
    int first_id;
    int last_id;
    proc proc_table[QUEUE_SIZE];
} queue;

typedef struct
{
    int context_switches;
    int next_id;
    int priority_mode;
    int executed_procs;
    int procs_on_progress
} scheduler;

int id;
int interruption;

// SIGUSR1
void func_sig_create()
{
    // Acessa fila
    // Seta msg_rec
    interruption = INTR_CREATE;
}
// SIGUSR2
void func_sig_close()
{
    // Acessa fila
    // Seta id
    interruption = INTR_CLOSE;
}

void initialize_scheduler(scheduler *s, int mode)
{
    s->context_switches = 0;
    s->next_id = 1;
    s->priority_mode = mode;
    s->executed_procs = 0;
    s->procs_on_progress = 0;
}

void initialize_queue(queue *q)
{
    q->first_id = -1;
    q->last_id = -1;
    q->size = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
        q->proc_table[i].pid = -1;
}

void add_proc_to_table(proc p, queue *q) {
    q->size++;
    p.next_id = q->last_id;

    int id = -1;
    for (int i = 0; i < QUEUE_SIZE; i++) {
        if (q->proc_table[i].pid == -1) {
            id = i;
            break;
        }
    }
    if (id >= 0) {
        q->proc_table[id] = p;
        if (q->size == 1)
            q->first_id = id;
        else
            q->proc_table[q->last_id].next_id = id;
        q->last_id = id;
    } else {
        printf("erro, fila sem espaco para adicionar novo processo\n");
    }
    
}

void add_proc(msg *m, scheduler *s, queue *q)
{
    // Precisa de todas as filas
    int pid;
    if((pid = fork()) < 0) {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0) {
        kill(getpid(), SIGTSTP);
        char s[] = "./";
        strcat(s, m->file_name);
        execlp(s, m->file_name, (char*) 0);
    }

    proc p;
    p.context_switches = 0;
    p.id = s->next_id;
    p.next_id = -1;
    p.pid = pid;
    p.start_time = time(0);

    if (m->priority == 1) {
        add_proc_to_table(p, q);
    }

    printf("processo %d adicionado", p.next_id);
    s->next_id = s->next_id + 1;
    s->procs_on_progress = s->procs_on_progress + 1;
}

void realocate_proc(scheduler *s, queue *q)
{
    // Precisa de todas as filas
    // DOUBT: Como vou saber qual o processo atual que está sendo executado e em que fila está?
    proc p;
    p.context_switches = 0;
    p.id = s->next_id;
    p.next_id = -1;
    // p.pid = pid;
    p.start_time = time(0);

}

int main()
{
    // Receber política
    srand(time(NULL));

    signal(SIGUSR1, func_sig_create);
    signal(SIGUSR2, func_sig_close);

    int alarm_time;
    scheduler *scheduler = malloc(sizeof(scheduler));
    queue *high_priority = malloc(sizeof(queue));

    initialize_scheduler(scheduler, 2);
    initialize_queue(high_priority);

    // Cria fila de mensagem para criar procos
    // Cria fila de mensagem para fechar procos

    while (1)
    {
        while (scheduler->procs_on_progress == 0)
        {
            // Espera ocupada
        }
        // Checa primeira fila não vazia
        // Seta alarme
        kill(high_priority->proc_table[high_priority->first_id].pid, SIGCONT);
        // Se o alarme for concluído
        //// Checa se não há interrupção
        ////// Se houver trata
        //// Realoca proc
        // Se o alarme for interrompido
        //// Trata a interrupção
        //// Depois continua proc
        // Talvez seja interessante fazer um loop caso haja várias interrupções
    }

    // TODO: Descobrir o que acontece quando acontece uma interrupção enquanto acontece um sleep
}


/**
 * @brief TODO
 *  - Checar qnts processos estão sendo criados no ex05
 *  - Definir etapas de teste
 *  - Começar a testar
 */