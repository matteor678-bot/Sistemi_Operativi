#ifndef _HEADER_H_
#define _HEADER_H_

#include "monitor_hoare.h"

typedef struct {

    int buffer;

    int buffer_libero; //1 se libero, 0 se pieno
    int buffer_occupato; //0 se libero, 1 se pieno

    Monitor m;

} MonitorProdCons;

void init_monitor_prodcons(MonitorProdCons *p);
void produzione(MonitorProdCons *p, int elemento);
int consumazione(MonitorProdCons *p);
void remove_monitor_prodcons(MonitorProdCons *p);

typedef struct {
    long type;
    int val;
} Messaggio;

#endif