#include<string.h>
#include<stdio.h>

int main() {
    char file_name[] = "ex04";
    char s[] = "./";
    strcat(s, file_name);
    printf("%s", s);
}