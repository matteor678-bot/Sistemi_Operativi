typedef struct {

    int mutex; //ID del semaforo per realizzare il mutex del monitor
    int urgent_sem; //ID del del semaforo per realizzare la coda urgent
    int num_var_cond; //Numero di variabili condition
    int id_conds; //ID del gruppo sem associati alle var. cond
    int id_shared; //ID della memoria condivisa per i contatori delle variabili condition e della coda urgent
    int *cond_counts; //Array delle variabili condition_count
    int *urgent_count; //Contatore del numero di processi sospesi sulla coda urgent
} Monitor;

//Monitor e numero di variabili condition:
void init_monitor (Monitor*, int); 
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void remove_monitor(Monitor*);
void wait_condition(Monitor*,int);
void signal_condition(Monitor*,int);
int queue_condition(Monitor*,int); 