#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define QUEUE_SIZE 10

typedef struct
{
    int pid;
    int context_switches;
    time_t start_time;
    time_t end_time;
    char *file_name;
    int prev_id;
    int next_id;
} process;

typedef struct
{
    int size;
    int first_id;
    int last_id;
    process proc_table[QUEUE_SIZE];
} queue;

typedef struct
{
    int context_switches;
    int next_pid;
    queue *low_priority;  // low priority
    queue *high_priority; // high priority
    queue *mid_priority;  // mid priority
} os;

void add_to_proc_table(os *os, queue *q, char *file_name)
{
    // printf("add_to_proc_table\n");
    q->size++;
    int id = -1;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (q->proc_table[i].pid == -1)
        {
            id = i;
            break;
        }
    }
    if (id >= 0)
    {
        q->proc_table[id].start_time = time(0);
        q->proc_table[id].file_name = file_name;
        q->proc_table[id].pid = os->next_pid;
        q->proc_table[id].prev_id = q->last_id;
        q->proc_table[id].next_id = -1;
        // printf("add process %d, pos %d\n", q->proc_table[id].pid, id);
        if (q->size == 1)
            q->first_id = id;
        else
            q->proc_table[q->last_id].next_id = id;
        q->last_id = id;
    }
}

void add_process(os *os, int priority, char *file_name)
{
    // printf("add_process\n");
    if (priority == 1)
        add_to_proc_table(os, os->high_priority, file_name);
    else if (priority == 2)
        add_to_proc_table(os, os->high_priority, file_name);
    else
        add_to_proc_table(os, os->high_priority, file_name);
    os->next_pid++;
}

void delete_from_proc_table(queue *q, int i, int pid)
{
    q->size--;
    q->proc_table[i].pid = -1;
    if (q->proc_table[i].prev_id == -1)
        q->first_id = q->proc_table[i].next_id;
    else
    {
        q->proc_table[q->proc_table[i].prev_id].next_id = q->proc_table[i].next_id;
        if (q->proc_table[i].next_id == -1)
            q->last_id = q->proc_table[i].prev_id;
        else
            q->proc_table[q->proc_table[i].next_id].prev_id = q->proc_table[i].prev_id;
    }
    return;
}

void delete_process(os *os, int pid)
{
    for (int i = 0; i < QUEUE_SIZE; i++)
        if (os->high_priority->proc_table[i].pid == pid)
            delete_from_proc_table(os->high_priority, i, pid);
}

void check_queue_order(queue *q)
{
    int id = q->first_id;
    printf("fila ");
    while (id != -1)
    {
        printf("%d ", q->proc_table[id].pid);
        id = q->proc_table[id].next_id;
    }
    printf("\n");
}

void initialize_queue(queue *q)
{
    //q = (queue *) malloc(sizeof(queue));
    q->first_id = -1;
    q->last_id = -1;
    q->size = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
        q->proc_table[i].pid = -1;
}

void initialize_os(os *sys)
{
    sys->context_switches = 0;
    sys->next_pid = 0;
    sys->high_priority = malloc(sizeof(queue));
    initialize_queue(sys->high_priority);
    sys->mid_priority = malloc(sizeof(queue));
    initialize_queue(sys->mid_priority);
    sys->low_priority = malloc(sizeof(queue));
    initialize_queue(sys->low_priority);  //TODO: AQUI O ERRO
    
}

int main()
{
    os *os = malloc(sizeof(os));
    initialize_os(os);

    char *file_name = "oioi";
    add_process(os, 1, file_name);
    add_process(os, 1, file_name);
    delete_process(os, 1);
    add_process(os, 1, file_name);
    add_process(os, 1, file_name);
    check_queue_order(os->high_priority);
}
