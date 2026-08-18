#define MUTEX_DISPONIBILITA 0

#define DIM_BUFFER 80

#define NUM_CLIENTI 50

#define LIBERO 0
#define OCCUPATO 1
#define IN_AGGIORNAMENTO 2

typedef struct {
    unsigned int id_cliente;
    unsigned int stato;
} posto;

typedef struct {
    posto posti[DIM_BUFFER];
    unsigned int disponibilita;
} Teatro;

void prenotazione(Teatro * teatro, int sem_id);
void visualizzazione(Teatro * teatro);