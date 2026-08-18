#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) { 

    int pid = fork();

    if(pid == -1) {
        fprintf(stderr, "fork fallita\n");
        exit(1);
    }

    if(pid == 0) {
        printf("Questo è il figlio\n");
        exit(0);
    }

    if(pid > 0)  {
        printf("Questo è il padre. Il figlio ha PID = %d\n", pid);
        exit(0);
    }

}