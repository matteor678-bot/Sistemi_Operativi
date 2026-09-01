#include <pthread.h>

#define NUM_LETTORI 3
#define NUM_SCRITTORI_LENTI 2
#define NUM_SCRITTORI_VELOCI 2
#define ITERAZIONI 3

typedef struct {
    int buffer; //AQI: compreso tra 0 e 500

    pthread_mutex_t mutex; //ID del semaforo per realizzare il mutex del monitor
    pthread_cond_t condvar_lettori;
    pthread_cond_t condvar_scrittoriveloci;
    pthread_cond_t condvar_scrittorilenti;
    int cond_counts[3]; //Array delle variabili condition_count
    int num_lettori;
    int num_scrittoriveloci;
    int num_scrittorilenti;

} MonitorAria;

typedef struct {
    MonitorAria *monitor;
    int id;
} thread_args;

void inizializza_monitor(MonitorAria *m);
void distruggi_monitor(MonitorAria *m);

int lettore(MonitorAria *m);
void scrittore_veloce(MonitorAria *m, int valore);
void scrittore_lento(MonitorAria *m, int valore);

void *thread_lettore(void* arg);
void *thread_scrittore_veloce(void *arg);
void *thread_scrittore_lento(void *arg);
