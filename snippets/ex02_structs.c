#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define QUEUE_SIZE 10
#define RANDOM_MODE 1
#define STATIC_MODE 2
#define DYNAMIC_MODE 3
#define HIGH_PRIORITY 1
#define MID_PRIORITY 2
#define LOW_PRIORITY 3

typedef struct
{
    int pid;
    int context_switches;
    time_t start_time;
    time_t end_time;
    char file_name[20];
    int prev_id;
    int next_id;
    int priority;
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
    int priority_mode;
    queue high_priority; // high priority
    queue mid_priority;  // mid priority
    queue low_priority;  // low priority
} os;

queue initialize_queue(queue q)
{
    // q = (queue *) malloc(sizeof(queue));
    q.first_id = -1;
    q.last_id = -1;
    q.size = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
        q.proc_table[i].pid = -1;
    return q;
}

os initialize_os(os sys)
{
    sys.context_switches = 0;
    sys.next_pid = 0;
    sys.priority_mode = STATIC_MODE;
    sys.high_priority = initialize_queue(sys.high_priority);
    printf("size 1 %d\n", sys.high_priority.size);
    sys.mid_priority = initialize_queue(sys.mid_priority);
    sys.low_priority = initialize_queue(sys.low_priority);
    printf("size 1 %d\n", sys.high_priority.size);
}

queue add_to_proc_table(queue q, char *file_name, int priority, int pid, time_t start_time)
{
    // printf("add_to_proc_table\n");
    q.size++;
    int id = -1;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (q.proc_table[i].pid == -1)
        {
            id = i;
            break;
        }
    }
    if (id >= 0)
    {
        q.proc_table[id].pid = pid;
        strcpy(q.proc_table[id].file_name, file_name);
        q.proc_table[id].start_time = start_time;
        q.proc_table[id].prev_id = q.last_id;
        q.proc_table[id].next_id = -1;
        q.proc_table[id].priority = priority;
        printf("add process %d, pos %d\n", q.proc_table[id].pid, id);
        if (q.size == 1)
            q.first_id = id;
        else
            q.proc_table[q.last_id].next_id = id;
        q.last_id = id;
    }
    return q;
}

os add_process(os os, int priority, char *file_name)
{
    // printf("add_process\n");
    int pid = os.next_pid;
    time_t start_time = time(0);
    if (priority == HIGH_PRIORITY)
        os.high_priority = add_to_proc_table(os.high_priority, file_name, 1, pid, start_time);
    else if (priority == MID_PRIORITY)
        os.mid_priority = add_to_proc_table(os.mid_priority, file_name, 2, pid, start_time);
    else if (priority == LOW_PRIORITY)
        os.low_priority = add_to_proc_table(os.low_priority, file_name, 3, pid, start_time);
    os.next_pid++;

    return os;
}

queue delete_from_proc_table(queue q, int i, int pid)
{
    q.size--;
    q.proc_table[i].pid = -1;
    if (q.proc_table[i].prev_id == -1)
        q.first_id = q.proc_table[i].next_id;
    else
    {
        q.proc_table[q.proc_table[i].prev_id].next_id = q.proc_table[i].next_id;
        if (q.proc_table[i].next_id == -1)
            q.last_id = q.proc_table[i].prev_id;
        else
            q.proc_table[q.proc_table[i].next_id].prev_id = q.proc_table[i].prev_id;
    }
    return q;
}

void delete_process(os os, int pid)
{
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (os.high_priority.proc_table[i].pid == pid)
        {
            os.high_priority = delete_from_proc_table(os.high_priority, i, pid);
            return;
        }
        if (os.mid_priority.proc_table[i].pid == pid)
        {
            os.mid_priority = delete_from_proc_table(os.mid_priority, i, pid);
            return;
        }
        if (os.low_priority.proc_table[i].pid == pid)
        {
            os.low_priority = delete_from_proc_table(os.low_priority, i, pid);
            return;
        }
    }
}

os realocate_process(os os, int priority)
{
    int pid;
    char file_name[20];
    time_t start_time;
    // Pega pid, file_name, time_start e deleta processo
    if (priority == HIGH_PRIORITY)
    {
        pid = os.high_priority.proc_table[os.high_priority.first_id].pid;
        strcpy(file_name, os.high_priority.proc_table[os.high_priority.first_id].file_name);
        start_time = os.high_priority.proc_table[os.high_priority.first_id].start_time;
        delete_from_proc_table(os.high_priority, os.high_priority.first_id, pid);
    }
    // else if (priority == MID_PRIORITY)
    // {
    //     pid = os.mid_priority.proc_table[os.mid_priority.first_id].pid;
    //     file_name = os.mid_priority.proc_table[os.mid_priority.first_id].file_name;
    //     start_time = os.mid_priority.proc_table[os.mid_priority.first_id].start_time;
    //     delete_from_proc_table(os.mid_priority, os.mid_priority.first_id, pid);
    // }
    // else if (priority == LOW_PRIORITY)
    // {
    //     pid = os.low_priority.proc_table[os.low_priority.first_id].pid;
    //     file_name = os.low_priority.proc_table[os.low_priority.first_id].file_name;
    //     start_time = os.low_priority.proc_table[os.low_priority.first_id].start_time;
    //     delete_from_proc_table(os.low_priority, os.low_priority.first_id, pid);
    // }
    
    // Define nova prioridade
    if (os.priority_mode == RANDOM_MODE)
    {
        priority = rand() % 3 + 1;
    }
    else if (os.priority_mode == DYNAMIC_MODE)
    {
        priority = priority == LOW_PRIORITY ? LOW_PRIORITY : priority + 1;
    }

    // Add process na fila da prioridade
    if (priority == HIGH_PRIORITY)
        os.high_priority = add_to_proc_table(os.high_priority, file_name, priority, pid, start_time);
    else if (priority == MID_PRIORITY)
        os.mid_priority = add_to_proc_table(os.mid_priority, file_name, priority, pid, start_time);
    else if (priority == LOW_PRIORITY)
        os.low_priority = add_to_proc_table(os.low_priority, file_name, priority, pid, start_time);
        
    
}

void select_next_process(os os)
{
}

void check_queue_order(queue q)
{
    int id = q.first_id;
    printf("%d, fila ", q.size);
    while (id != -1)
    {
        printf("%d ", q.proc_table[id].pid);
        id = q.proc_table[id].next_id;
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));

    os os;
    os = initialize_os(os);

    char file_name = "oioi";
    os = add_process(os, 1, file_name);
    os = add_process(os, 1, file_name);
    check_queue_order(os.high_priority);
    os = realocate_process(os, 1);
    check_queue_order(os.high_priority);
    os = add_process(os, 1, file_name);
    os = add_process(os, 1, file_name);
    os = realocate_process(os, 1);
    check_queue_order(os.high_priority);
}


// TODO: testar realocate process, testar com processos em várias