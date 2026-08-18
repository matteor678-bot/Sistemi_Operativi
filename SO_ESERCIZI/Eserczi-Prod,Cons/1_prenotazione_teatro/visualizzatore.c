#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>

#include "procedure.h"

int main(void) {

    key_t chiave = ftok("/home/matteo/Scrivania", 'a');

    int ds_shm = shmget(chiave, sizeof(Teatro), 0);

    if(ds_shm<0) { perror("SHM errore"); exit(1); }

    Teatro * t;

    t = (Teatro *) shmat(ds_shm, NULL, 0);

    visualizzazione(t);

    return 0;
}