#include <stdio.h>

#include "procedure.h"

void inizializza(MonitorPortafoglio * m, int quantita_iniziale, int valore_iniziale) {
    m->quantita = quantita_iniziale;
    m->valore = valore_iniziale;
    pthread_mutex_init(&(m->mutex_port), NULL);
    pthread_cond_init(&(m->varcond_acquisto), NULL);
    pthread_cond_init(&(m->varcond_vendita), NULL);
}

void termina(MonitorPortafoglio * m) {
    pthread_mutex_destroy(&(m->mutex_port));
    pthread_cond_destroy(&(m->varcond_acquisto));
    pthread_cond_destroy(&(m->varcond_vendita));
}

void acquisto(MonitorPortafoglio * m, int quantita, int prezzo) {

    pthread_mutex_lock(&(m->mutex_port));

    while( prezzo < m->valore)
        pthread_cond_wait(&(m->varcond_acquisto), &(m->mutex_port));

    m->quantita += quantita;

    pthread_mutex_unlock(&(m->mutex_port));

}

void vendita(MonitorPortafoglio * m, int quantita, int prezzo) {

    pthread_mutex_lock(&(m->mutex_port));

    while(!((prezzo <= m->valore) && (m->quantita >= quantita)))
        pthread_cond_wait(&(m->varcond_vendita), &(m->mutex_port));

    m->quantita -= quantita;

    pthread_mutex_unlock(&(m->mutex_port));
}

void aggiorna(MonitorPortafoglio * m, int valore) {

    pthread_mutex_lock(&(m->mutex_port));

    m->valore = valore;

    pthread_cond_broadcast(&(m->varcond_vendita));
    pthread_cond_broadcast(&(m->varcond_acquisto));

    pthread_mutex_unlock(&(m->mutex_port));

}

int leggi(MonitorPortafoglio * m) {

    pthread_mutex_lock(&(m->mutex_port));

    int valore;

    valore = m->valore;

    pthread_mutex_unlock(&(m->mutex_port));

    return valore;
}