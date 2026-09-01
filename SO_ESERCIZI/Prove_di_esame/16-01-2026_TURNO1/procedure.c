#include "header.h"
#include <stdio.h>

#define varcond_produttori 0
#define varcond_consumatori 1

void init_monitor_prodcons(MonitorProdCons *p) {

    p->buffer = 0;
    p->buffer_libero = 1;
    p->buffer_occupato = 0;
    init_monitor(&(p->m), 2);

}


void produzione(MonitorProdCons *p, int elemento) {

    enter_monitor(&(p->m));

    printf("Ingresso monitor - produzione\n");

    if(p->buffer_occupato == 1) {

        printf("Sospensione - produzione\n");
        wait_condition(&(p->m), varcond_produttori);
        printf("Riattivazione - produzione\n");
    }

    p->buffer = elemento;
    p->buffer_libero = 0;
    p->buffer_occupato = 1;

    printf("Produzione (%d)\n", elemento);

    signal_condition(&(p->m), varcond_consumatori);

    leave_monitor(&(p->m));

    printf("Uscita monitor - produzione\n");

}


int consumazione(MonitorProdCons *p) {
    
    int valore;
    
    enter_monitor(&(p->m));

    printf("Ingresso monitor - consumazione\n");

    if(p->buffer_libero == 1) {

        printf("Sospensione - consumazione\n");
        wait_condition(&(p->m), varcond_consumatori);
        printf("Riattivazione - consumazione\n");
    }

    valore = p->buffer;
    p->buffer_libero = 1;
    p->buffer_occupato = 0;

    printf("Consumazione (%d)\n", valore);

    signal_condition(&(p->m), varcond_produttori);

    leave_monitor(&(p->m));

    printf("Uscita monitor - consumazione\n");

    return valore;

}


void remove_monitor_prodcons(MonitorProdCons *p) {

    p->buffer = 0;
    p->buffer_libero = 0;
    p->buffer_occupato = 0;
    remove_monitor(&(p->m));

}
