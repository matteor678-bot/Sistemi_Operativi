#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "procedure.h"
#include "semafori.h"

void prenotazione(Teatro * teatro, int sem_id) {

    int n_posti;

    srand(time(NULL));
    sleep(rand()%6);

    n_posti = 1 + rand() % 4;

    int indice = 0;
    int indici[n_posti];

    Wait_Sem(sem_id, MUTEX_DISPONIBILITA); //Inizio sezione critica

    if((teatro->disponibilita)<n_posti) {
        printf("ERRORE: Disponibilità esaurita");
        Signal_Sem(sem_id, MUTEX_DISPONIBILITA);
        exit(1);
    }

    for(int i = 0; i<n_posti; i++) {

        while(indice<DIM_BUFFER && teatro->posti[indice].stato != LIBERO) {
            indice++;
        }

        teatro->posti[indice].stato = IN_AGGIORNAMENTO;
        indici[i] = indice;

        indice++;
    }

    teatro->disponibilita = (teatro->disponibilita) - n_posti;

    Signal_Sem(sem_id, MUTEX_DISPONIBILITA); //Fine sezione critica

    //Operazione di prenotazione effettuata in concorrenza tra i processi clienti:
    for(int i = 0; i<n_posti; i++) {
        teatro->posti[indici[i]].id_cliente = getpid();
        teatro->posti[indici[i]].stato = OCCUPATO;
    }

}

void visualizzazione(Teatro * teatro) {
    char * stato_posto;

    while(true) {

        sleep(1);
        printf("STATO DEI POSTI NEL TEATRO:\n");
        for( int i = 0; i<DIM_BUFFER; i++) {
            switch(teatro->posti[i].stato) {
                case LIBERO: stato_posto = "LIBERO";
                             break;
                case OCCUPATO: stato_posto = "OCCUPATO";
                               break;
                case IN_AGGIORNAMENTO: stato_posto = "IN AGGIORNAMENTO";
                                       break;
            }
            printf("POSTO[%d]   STATO: %s   ID CLIENTE: %d\n", i, stato_posto, teatro->posti[i].id_cliente);
        }

    }
}