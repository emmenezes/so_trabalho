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

#define IPC_ID 2353
#define QUEUE_SIZE 15
#define RANDOM_MODE 1
#define STATIC_MODE 2
#define DYNAMIC_MODE 3
#define INTR_ALARM 1

typedef struct
{
    int id;
    char file_name[30];
} msg;

typedef struct
{
    int pid;
    int id;
    int context_switches;
    time_t start_time;
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
int idfila;
msg msg_rcv = {.id = -1, .file_name = ""};
struct msqid_ds info;

void func_sig_alarm()
{
    interruption = INTR_ALARM;
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
    printf("i\tid\tpid\tprev_id\tnext_id\n");
    for (int i = 0; i < QUEUE_SIZE; i++)
        printf("%d\t%d\t%d\t%d\t%d\n", i, t[i].id, t[i].pid, t[i].prev_id, t[i].next_id);
}

void check_queues(queue *q)
{
    printf("queue\tsize\tfirst\tlast\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%d\t%d\t%d\t %d\n", i, q[i].size, q[i].first_id, q[i].last_id);
    }
}

void add_proc(scheduler *s, queue *q, proc *t)
{
    int pid;
    time_t now;
    if ((pid = fork()) < 0)
    {
        printf("erro no fork\n");
        exit(1);
    }
    if (pid == 0)
    {
        char s[] = "./";
        strcat(s, msg_rcv.file_name);
        kill(getpid(), SIGSTOP);
        execlp(s, msg_rcv.file_name, (char *)0);
    }
    int priority = msg_rcv.id - 101;
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
            printf("proc %d na pos %d, pid %d, prioridade %d\n", t[i].id, i, t[i].pid, priority + 1);
            s->next_id++;
            break;
        }
    }
    if (id == -1)
        printf("\nerro, fila sem espaco para adicionar novo processo\n\n");
}

void relocate_proc(scheduler *s, queue *q, proc *t)
{
    int id = s->proc_executing;
    int priority = t[id].priority;
    q[priority].size--;
    q[priority].first_id = t[id].next_id;
    if (q[priority].first_id != -1)
        t[q[priority].first_id].prev_id = -1;
    if (q[priority].last_id == id)
        q[priority].last_id = t[id].next_id;
    t[id].next_id = -1;
    if (s->priority_mode == RANDOM_MODE)
    {
        priority = rand() % 3;
        printf("proc %d tem agora prioridade %d\n", t[id].id, priority + 1);
    }
    else if (s->priority_mode == DYNAMIC_MODE)
    {
        if (priority != 2)
            priority++;
        printf("proc %d tem agora prioridade %d\n", t[id].id, priority + 1);
    }
    q[priority].size++;
    t[id].prev_id = q[priority].last_id;
    t[id].priority = priority;
    if (q[priority].size == 1)
        q[priority].first_id = id;
    else
        t[q[priority].last_id].next_id = id;
    q[priority].last_id = id;
}

void select_proc_to_exec(scheduler *s, queue *q, proc *t)
{
    int id = -1;
    for (int i = 0; i < 3; i++)
    {
        if (q[i].size > 0)
        {
            id = q[i].first_id;
            t[id].context_switches++;
            break;
        }
    }
    printf("\n");
    s->proc_executing = id;
    s->context_switches++;
}

void kill_proc(scheduler *s, queue *q, proc *t)
{
    int id = -1, estado;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (t[i].id == msg_rcv.id)
        {
            id = i;
            break;
        }
    }
    if (id == -1)
    {
        printf("proc %d nao encontrado\n", msg_rcv.id);
        return;
    }
    time_t now;
    time_t end_time = time(&now);
    s->procs_on_progress--;
    int priority = t[id].priority;
    if (t[id].prev_id != -1)
        t[t[id].prev_id].next_id = t[id].next_id;
    if (t[id].next_id != -1)
        t[t[id].next_id].prev_id = t[id].prev_id;
    q[priority].size--;
    if (q[priority].first_id == id)
        q[priority].first_id = t[id].next_id;
    if (q[priority].last_id == id)
        q[priority].last_id = t[id].prev_id;
    kill(t[id].pid, SIGKILL);
    wait(&estado);
    printf("proc %d cancelado, tempo total %ld, trocas de contexto %d\n", t[id].id, end_time - t[id].start_time, t[id].context_switches);
    t[id].id = -1;
}

void remove_proc(scheduler *s, queue *q, proc *t)
{
    s->procs_on_progress--;
    s->executed_procs++;
    int id = s->proc_executing;
    int priority = t[id].priority;
    q[priority].size--;
    q[priority].first_id = t[id].next_id;
    t[q[priority].first_id].prev_id = -1;
    time_t now;
    time_t end_time = time(&now);
    printf("proc %d concluido, temp total %ld, trocas de contexto %d\n", t[id].id, end_time - t[id].start_time, t[id].context_switches);
    t[id].id = -1;
}

void kill_prog(scheduler *s, proc *t)
{
    int estado;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (t[i].id != -1)
        {
            kill(t[i].pid, SIGKILL);
            t[i].id = -1;
            wait(&estado);
        }
    }
    printf("%d procs encerrados\n%d procs cancelados\n%d trocas de contexto\n", s->executed_procs, s->procs_on_progress, s->context_switches);
}

int main(int argc, char *argv[])
{
    char *p;
    long arg = 0;
    int mode;
    if (argc == 2)
    {
        arg = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0)
        {
            printf("Argumento invalido, insera apenas o numero do processo que deseja encerrar\n");
            return 1;
        }
        if (arg == 1 || arg == 2 || arg == 3)
        {
            mode = arg;
        }
        else
        {
            printf("Argumento invalido, insira 1 para RANDOM_MODE, 2 para STATIC_MODE, 3 para DYNAMIC_MODE 3\n");
            return 1;
        }
    }
    else
    {
        printf("Quantidade de argumentos invalida, insira 1 para RANDOM_MODE, 2 para STATIC_MODE, 3 para DYNAMIC_MODE 3\n");
        return 1;
    }

    srand(time(NULL));

    signal(SIGALRM, func_sig_alarm);
    if ((idfila = msgget(IPC_ID, IPC_CREAT | 0x1FF)) < 0)
    {
        printf("erro na criação da fila\n");
        return 1;
    }

    int timer = 10, estado, ret, id, pid, exit = 0;
    scheduler *scheduler = malloc(sizeof(scheduler));
    queue *queues = malloc(3 * sizeof(queue));
    proc *proc_table = malloc(QUEUE_SIZE * sizeof(proc));

    initialize_scheduler(scheduler, mode);
    initialize_queue(queues);
    initialize_proc_table(proc_table);

    while (1)
    {
        if (scheduler->procs_on_progress == 0)
        {
            if (msgrcv(idfila, &msg_rcv, sizeof(msg_rcv), 0, 0) > 0)
            {
                if (msg_rcv.id == -1)
                {
                    kill_prog(scheduler, proc_table);
                    exit = 1;
                    break;
                }
                else if (msg_rcv.id > 100)
                    add_proc(scheduler, queues, proc_table);
                else
                    kill_proc(scheduler, queues, proc_table);
            }
        }
        else
        {
            // check_queues(queues);
            select_proc_to_exec(scheduler, queues, proc_table);
            id = proc_table[scheduler->proc_executing].id;
            pid = proc_table[scheduler->proc_executing].pid;
            kill(pid, SIGCONT);
            printf("proc %d executando, prioridade %d\n", id, proc_table[scheduler->proc_executing].priority + 1);
            alarm(timer);
            while (1)
            {
                if (msgrcv(idfila, &msg_rcv, sizeof(msg_rcv), 0, 0) > 0)
                {
                    if (msg_rcv.id == -1)
                    {
                        kill_prog(scheduler, proc_table);
                        exit = 1;
                        break;
                    }
                    else if (msg_rcv.id > 100)
                        add_proc(scheduler, queues, proc_table);
                    else
                    {
                        kill_proc(scheduler, queues, proc_table);
                        if (msg_rcv.id == id)
                        {
                            alarm(0);
                            break;
                        }
                    }
                }
                else
                {
                    if (interruption == INTR_ALARM)
                    {
                        interruption = 0;
                        ret = waitpid(pid, &estado, WNOHANG);
                        if (ret == pid)
                        {
                            remove_proc(scheduler, queues, proc_table);
                        }
                        else
                        {
                            kill(pid, SIGSTOP);
                            relocate_proc(scheduler, queues, proc_table);
                        }
                    }
                    break;
                }
            }
        }
        if (exit)
            break;
    }
    msgctl(idfila, IPC_RMID, &info);
    printf("programa encerrado\n");
}
