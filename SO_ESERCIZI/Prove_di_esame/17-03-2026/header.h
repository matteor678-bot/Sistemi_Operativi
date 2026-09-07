#ifndef _HEADER_H
#define _HEADER_H

#define X 1
#define Y 2
#define MESSAGGIO 3

#include <pthread.h>

typedef struct {
    int buffer_x;
    int buffer_y;
    pthread_mutex_t buffervuoto_x;
    pthread_mutex_t bufferpieno_x;
    pthread_mutex_t buffervuoto_y;
    pthread_mutex_t bufferpieno_y;
} coppia_buffer;

typedef struct {
    long type;
    int val;
} messaggio_sensore;

typedef struct {
    long type;
    int val;
} messaggio_attuatore;

void sensore_x(int coda_sensori);
void sensore_y(int coda_sensori);
void planner(int coda_sensori, int coda_attuatore);
void attuatore(int coda_attuatore);

void init_coppia_buffer(coppia_buffer * b);
void remove_coppia_buffer(coppia_buffer * b);
void produci(coppia_buffer * buffer, int tipo, int valore);
void consuma(coppia_buffer * buffer, int * valore_x, int * valore_y);

#endif