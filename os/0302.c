#include <sys/syscall.h>
#include <string.h>
#include <malloc.h>

main () {

    int i;
    int SIZE = 8888;
    for (i = 0; i < 100; i++) {
        int* p = (int*) malloc(sizeof(int) * SIZE);
        memset(p, 0, sizeof(int) * SIZE);
    }
    syscall(316);
}
