#define MUTEX_NUMLETTORI 0
#define MUTEX_LETTORI_SCRITTORI 1

typedef struct {
     int val_1;
     int val_2;
} buffer;

typedef struct {
    buffer b;
    int numlettori;
} Buffer;

void InizioLettura(int, Buffer*);
void FineLettura(int, Buffer*);
void InizioScrittura(int);
void FineScrittura(int);