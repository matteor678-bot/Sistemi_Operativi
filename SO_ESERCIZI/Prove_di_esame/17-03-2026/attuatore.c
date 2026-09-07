#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

void attuatore(int coda_attuatore) {

    printf("[Attuatore] Avviato...\n");
    messaggio_attuatore m;

    for(int i = 0; i < 10; i++) {

        /* TBD: Ricevere un messaggio con il valore sulla coda dell'attuatore */
        msgrcv(coda_attuatore, (void *)&m, sizeof(messaggio_attuatore)-sizeof(long), MESSAGGIO, 0);

        int valore = m.val;

        printf("[Attuatore] Ricevuto %d\n", valore);

    }

}