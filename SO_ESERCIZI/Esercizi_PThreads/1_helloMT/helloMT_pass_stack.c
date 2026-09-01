#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void * PrintHello(void * arg) { //start routine

    int * int_ptr = (int *)arg;
    printf("\n%d: Hello World! (arg address: %p)\n", *int_ptr, int_ptr);
    pthread_exit(NULL); // terminazione thread "figlio"


}

int main (int argc, char *argv[]) {

    pthread_t threads[NUM_THREADS];

    int rc;
    for(int i=0; i<NUM_THREADS; i++) {
        printf("Creating thread %d\n", i);

            /* Allocazione dell'argomento da passare al thread tramite stack del thread padre. 
            Questo tipo di allocazione genera dei warning dal punto di vista del typecast -> tipo di 
            passaggio dei parametri altamente sconsigliato!!!*/

            rc = pthread_create(&threads[i], NULL, PrintHello,(void *)&i);
            if(rc!=0) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
    }

    pthread_exit(NULL); // terminazione del thread "padre"
}