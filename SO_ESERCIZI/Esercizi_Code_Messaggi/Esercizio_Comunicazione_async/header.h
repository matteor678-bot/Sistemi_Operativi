#ifndef _HEADER
#define _HEADER
#define MESSAGGIO 1

typedef char msg[40];

typedef struct {
    long type;
    msg mess;
} Messaggio;

void Produttore(int queue, char* m);
void Consumatore(int queue);
void printMsgInfo(int queue);

#endif