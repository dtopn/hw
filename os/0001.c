#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int value = 5;
int main() {
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        /* child process */ value += 15;
    }
    else if (pid > 0) {
        /* parent process */
        sleep(5);
        wait(NULL);
        printf("PARENT: value = %d", value); /* LINE A */
        exit(0);
    }
}
