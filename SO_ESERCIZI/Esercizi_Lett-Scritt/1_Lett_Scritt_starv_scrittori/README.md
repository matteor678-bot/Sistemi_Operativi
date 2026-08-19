## Lettore-Scrittori singolo buffer con starvation degli scrittori

Nel problema **lettori-scrittori** abbiamo due categorie di processi:

- **Lettori**, che leggono un messaggio su di una risorsa condivisa

- **Scrittori**, che scrivono il messaggio dalla risorsa condivisa

I vincoli del problema sono:

- I processi lettori possono accedere *contemporaneamente* alla risorsa;

- I processi scrittori hanno accesso *esclusivo* alla risorsa;

- I lettori e scrittori si *escludono mutuamente* dall'uso della risorsa.

In particolare, in questo esercizio affrontiamo lo scenario in cui abbiamo **starvation dei soli scrittori**.

---

### Esercizio
*Scrivere un’applicazione concorrente che implementi il problema dei Lettori/Scrittori con starvation degli scrittori. Il programma crei dei processi che agiscano da lettori e scrittori utilizzando un unico buffer di memoria in cui sono memorizzati valori di tipo long. Il buffer di memoria è creato attraverso una shared memory e la sincronizzazione tra produttori e consumatori deve avvenire tramite l'utilizzo di semafori.*

Per risolvere tale problema, si utilizza una variable condivisa `numlettori` per contare il numero di lettori che contemporaneamente accedono alla risorsa. Solo quando nessun lettore è in attesa di leggere (`numlettori == 0`), gli scrittori potranno accedere (sempre uno alla volta) alla risorsa condivisa per poter scrivere.

Per disciplinare lettori e scrittori si utilizzano 2 semafori:
- `MUTEX_NUMLETTORI` per gestire l’accesso alla variabile numlettori in mutua esclusione, da parte dei lettori. Tale semaforo è inizializzato a `1`.

- `MUTEX_LETTORI_SCRITTORI` per garantire la mutua esclusione tra i processi lettori e scrittori e tra i soli processi scrittori. Tale semaforo è inizializzato a `1`.

Le operazioni di lettura sono "protette" dalle procedure di `Inizio_Lettura()` e `Fine_Lettura()`, mentre le operazioni di scrittura sono "protette" da `Inizio_Scrittura()` e `Fine_Scrittura()`.

Un processo lettore attende solo se la risorsa è occupata da un processo scrittore. Un processo scrittore può accedere alla risorsa solo se questa è libera.

Questa particolare strategia di sincronizzazione può tuttavia può provocare condizioni di attesa indefinita(starvation) per i processi scrittori.



