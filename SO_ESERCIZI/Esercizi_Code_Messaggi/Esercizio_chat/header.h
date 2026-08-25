#ifndef _HEADER_
#define _HEADER_
#define MESSAGGIO 1

typedef char msg[40];

typedef struct {
    long tipo;
    msg mess;
} Messaggio;

void  Mittente(int , char *);
Messaggio* Ricevente(int);

#endif 