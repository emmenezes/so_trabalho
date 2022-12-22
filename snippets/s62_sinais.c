#include<signal.h>
#include<stdio.h>
#include <unistd.h>

void func_sigusr1() {
    printf("recebi sigusr1\n");
}

void func_sigusr2() {
    printf("recebi sigusr2\n");
}

int main() {
    signal(SIGUSR1, func_sigusr1);
    signal(SIGUSR2, func_sigusr2);
    kill(getpid(), SIGUSR2);
    sleep(10);
    kill(getpid(), SIGUSR1);
}