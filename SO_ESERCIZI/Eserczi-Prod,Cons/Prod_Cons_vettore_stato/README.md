# Produttore-Consumatore consumatore con pool di buffer gestito con vettore di stato

---

#### Esercizio
*Scrivere un’applicazione concorrente che implementi il problema dei Produttori/Consumatori. Il programma crei dei processi che agiscano da produttore e consumatore utilizzando un pool di buffer in cui sono memorizzati valori di tipo intero. Tale pool di buffer deve essere gestito con vettore di stato. Il pool di buffer è creato attraverso una shared memory e la sincronizzazione tra produttori e consumatori deve avvenire tramite l'utilizzo di semafori.*

La soluzione con pool di buffer gestito come coda circolare può penalizzare produttori o consumatori veloci in presenza di produttori o consumatori lenti. Questo può accadere, ad esempio, quando i messaggi prodotti hanno dimensione variabile.

I vincoli che caratterizzano il problema produttore-consumatore con pool di buffer con vettore di stato sono gli stessi per il problema a singolo buffer, ovvero che un produttore non può produrre se non c'è spazio disponibile, mentre un consumatore non può consumare se non ci sono valori disponibili.

In questo caso però, ci si avvale di un vettore di stato ausiliare per acquisire il lasciapassare a produrre o consumare una specifica locazione del pool di buffer in maniera concorrente. L'accesso a tale vettore di stato è in mutua esclusione, e dopo aver acquisito un buffer del pool, produttori e consumatori procedono in concorrenza.

![alt text](image.png)

Il pull di buffer e il vettore di stato sono implementati attraverso la seguente struttura dati:

```c
struct prodcons {
    int buffer[DIM_BUFFER];
    int stato[DIM_BUFFER];
};
```

dove:


- `buffer[DIM_BUFFER]`, un array di elementi di tipo `int`(tipo del messaggio depositato dai produttori) contenente i valori prodotti;

- `stato[DIM_BUFFER]`, un array di elementi di tipo intero. Il valore i-esimo, `stato[i]`, può assumere i seguenti tre valori:

  - `BUFFER_VUOTO` – la cella `buffer[i]` non contiene alcun valore prodotto;
  - `BUFFER_PIENO` – la cella `buffer[i]` contiene un valore prodotto e non ancora consumato;
  - `BUFFER_INUSO` – il valore della cella `buffer[i]` contiene un valore in uso da un processo attivo, consumatore o produttore.

La struttura prodcons è condivisa tra i processi produttori e consumatori tramite shared memory.

Come per il problema con coda circolare, per la sincronizzazione dei processi produttore e consumatore si utilizzano quattro semafori:

- `SPAZIO_DISPONIBILE`, che indica la presenza di spazio disponibile in coda per la produzione di un messaggio. `SPAZIO_DISPONIBILE` ha valore iniziale pari a `DIM_BUFFER` (dimensione della coda)

- `MESSAGGIO_DISPONIBILE`, che indica il numero di messaggi presenti in coda. `MESSAGGIO_DISPONIBILE` ha valore iniziale pari a `0`.

- `MUTEX_C` per gestire la competizione per le operazioni di consumo, inizializzato a `1`.

- `MUTEX_P` per gestire la competizione per le operazioni di produzione, inizializzato a `1`.

La produzione ed il consumo avvengono rispettivamente all'interno delle procedure:
```c
void produttore(struct prodcons *, int);
void consumatore(struct prodcons *, int);
```
dove, il primo argomento è un puntatore alla struttura che gestisce il pool di buffer e il vettore di stato memorizzati nella shared memory creata, mentre il secondo parametro indica il descrittore del semaforo da utilizzare per le operazioni di wait su semaforo (i.e., Wait_Sem) e signal su semaforo (i.e., Signal_Sem) necessarie per la cooperazione e competizione tra produttore e consumatore. Il valore prodotto è un intero generato tramite la funzione rand().

