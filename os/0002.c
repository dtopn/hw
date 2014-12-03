#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int i = 0;
int main() {
    pid_t pid;
    pid = fork();

    while(i < 3) {
        pid = fork();
        printf("%d\n", i);
        i++;
    }

}
