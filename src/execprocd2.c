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

#define QUEUE_SIZE 5
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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    int priority;
    char *file_name;
} msg;

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
} queue;

typedef struct
{
    int context_switches;
    int next_id;
    int priority_mode;
    int executed_procs;
    int procs_on_progress;
    int proc_executing;
} scheduler;

int rm_proc_id = 0;
int interruption = 0;
msg msg_rcv = {.priority = -1, .file_name = ""};

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
    s->proc_executing = -1;
    
}

void initialize_queue(queue *q)
{
    for (int i = 0; i < 3; i++)
    {
        q[i].first_id = -1;
        q[i].last_id = -1;
        q[i].size = 0;
    }
}

void initialize_proc_table(proc *t)
{
    for (int i = 0; i < QUEUE_SIZE; i++)
        t[i].id = -1;
}

void check_proc_table(proc *t)
{
    printf("i, id, pid, prev_id, next_id\n");
    for (int i = 0; i < QUEUE_SIZE; i++)
        printf("\t%d\t%d\t%d\t%d\t%d\n", i, t[i].id, t[i].priority, t[i].prev_id, t[i].next_id);
}

void check_queues(queue *q) 
{
    for (int i = 0; i < 3; i++) {
        printf("size\tfirst\tlast\t##%d\n", i);
        printf("%d\t%d\t %d\n", q[i].size, q[i].first_id, q[i].last_id);
    }
}

void add_proc(scheduler *s, queue *q, proc *t)
{
    // Precisa de todas as filas
    int pid;
    time_t now;
    if ((pid = fork()) < 0)
    {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0)
    {
        kill(getpid(), SIGKILL);    // TODO: Processo se matando
        char *s = "./";
        strcat(s, msg_rcv.file_name);
        execlp(s, msg_rcv.file_name, (char *)0);
    }

    int priority = msg_rcv.priority -1;
    int id = -1;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (t[i].id == -1)
        {
            id = i;
            s->procs_on_progress++;
            q[priority].size++;
            t[i].context_switches = 0;
            t[i].id = s->next_id;
            t[i].next_id = -1;
            t[i].pid = pid;
            t[i].start_time = time(&now);
            t[i].priority = priority;
            t[i].prev_id = q[priority].last_id;
            if (q[priority].size == 1)
                q[priority].first_id = i;
            else
                t[q[priority].last_id].next_id = i;
            q[priority].last_id = i;
            printf("processo %d adicionado na pos %d no tempo %ld\n", t[i].id, i, t[i].start_time);
            s->next_id++;
            break;
        }
    }
    if (id == -1)
        printf("erro, fila sem espaco para adicionar novo processo\n");
}

void relocate_proc(scheduler *s, queue *q, proc *t)
{
    int id = s->proc_executing;
    int priority = t[id].priority;
    q[priority].size--;
    q[priority].first_id = t[id].next_id;
    t[q[priority].first_id].prev_id = -1;
    t[id].next_id = -1;

    if (s->priority_mode == RANDOM_MODE) {
        // TODO
    } else if (s->priority_mode == DYNAMIC_MODE) {
        // TODO
    }

    q[priority].size++;
    t[id].prev_id = q[priority].last_id;
    if (q[priority].size == 1)
        q[priority].first_id = id;
    else
        t[q[priority].last_id].next_id = id;
    q[priority].last_id = id;
}

void select_proc_to_exec(scheduler *s, queue *q, proc *t) {
    int id;
    for (int i = 0; i < 3; i++) {
        id = q[i].first_id;
        if (id != -1) {
            t[id].context_switches++;
            break;
        }
    }
    s->proc_executing = id;
    s->context_switches++;
}

void kill_proc(scheduler *s, queue *q, proc *t) {
    s->procs_on_progress--;
    int id = -1, estado;
    for (int i = 0; i < QUEUE_SIZE; i++) {
        printf("%d id %d\n", i, t[i].id);
        if (t[i].id == rm_proc_id) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        printf("proc %d nao encontrado\n\n", rm_proc_id);
        return;
    }
    time_t now;
    time_t end_time = time(&now);
    int priority = t[id].priority;
    if (s->proc_executing == id) {
        // alarm(0); ressetar alarme
    }
    
    t[t[id].prev_id].next_id = t[id].next_id;
    t[t[id].next_id].prev_id = t[id].prev_id;
    q[priority].size--;
    if (q[priority].first_id == id)
        q[priority].first_id = t[id].next_id;
    if (q[priority].last_id == id)
        q[priority].last_id = t[id].prev_id;
    t[id].id = -1;
    printf("proc %d encerrado\ntemp total %ld\ntrocas de contexto %d\n\n", t[id].id, end_time - t[id].start_time, t[id].context_switches);
    kill(t[id].pid, SIGKILL);
    wait(&estado);
}

void remove_proc(scheduler *s, queue *q, proc *t) {
    
}

void kill_prog(scheduler *s, proc *t) 
{

}

int main()
{
    // Receber política
    srand(time(NULL));

    signal(SIGUSR1, func_sig_create);
    signal(SIGUSR2, func_sig_close);

    int alarm_time, estado;
    scheduler *scheduler = malloc(sizeof(scheduler));
    queue *queues = malloc(3 * sizeof(queue));
    proc *proc_table = malloc(QUEUE_SIZE * sizeof(proc));

    initialize_scheduler(scheduler, STATIC_MODE);
    initialize_queue(queues);
    initialize_proc_table(proc_table);

    msg_rcv.file_name = "ex04";
    msg_rcv.priority = 1;

    add_proc(scheduler, queues, proc_table);
    add_proc(scheduler, queues, proc_table);
    add_proc(scheduler, queues, proc_table);
    select_proc_to_exec(scheduler, queues, proc_table);
    relocate_proc(scheduler, queues, proc_table);
    rm_proc_id = 1;
    sleep(1);
    kill_proc(scheduler, queues, proc_table);
    add_proc(scheduler, queues, proc_table);
    check_queues(queues);
    select_proc_to_exec(scheduler, queues, proc_table);
    relocate_proc(scheduler, queues, proc_table);
    // check_queues(queues);
    select_proc_to_exec(scheduler, queues, proc_table);
    printf("proc %d\n", scheduler->proc_executing);
    wait(&estado);

    // Cria fila de mensagem para criar procos
    // Cria fila de mensagem para fechar procos

    // while (1)
    // {
    //     while (scheduler->procs_on_progress == 0)
    //     {
    //         // Espera ocupada
    //     }
    //     // Checa primeira fila não vazia
    //     // Seta alarme
    //     kill(proc_table[high_priority->first_id]->pid, SIGCONT);
    //     // Se o alarme for concluído
    //     //// Checa se não há interrupção
    //     ////// Se houver trata
    //     //// Realoca proc
    //     // Se o alarme for interrompido
    //     //// Trata a interrupção
    //     //// Depois continua proc
    //     // Talvez seja interessante fazer um loop caso haja várias interrupções
    // }

    // TODO: Descobrir o que acontece quando acontece uma interrupção enquanto acontece um sleep
}

/**
 * @brief TODO
 *  - Checar qnts processos estão sendo criados no ex05
 *  - Definir etapas de teste
 *  - Começar a testar
 *
 * [X] Conferir se inicializações funcionam
 * [X] Testar add_proc
 * [X] Testar select_proc_to_exec
 * [X] Criar relocate_proc
 * [X] Testar relocate_proc
 * [X] Testar select_proc_to_exec
 * [X] Testar com mais processos
 * [ ] Criar kill proc
 * [ ] Testar o kill proc
 * [ ] Criar terminar programa
 */