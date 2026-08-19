## Lettori-Scrittori singolo buffer con starvation di entrambi
---
Nel problema **lettori-scrittori** abbiamo due categorie di processi:

- **Lettori**, che leggono un messaggio su di una risorsa condivisa
- **Scrittori**, che scrivono il messaggio dalla risorsa condivisa

I vincoli del problema sono:
- i processi lettori possono accedere *contemporaneamente* alla risorsa;
- i processi scrittori hanno accesso *esclusivo* alla risorsa;
- i lettori e scrittori si *escludono mutuamente* dall'uso della risorsa.

In particolare, in questo esercizio affrontiamo lo scenario in cui abbiamo **starvation di lettori e di scrittori**.

#### Esercizio
*Scrivere un’applicazione concorrente che implementi il problema dei Lettori/Scrittori con starvation di entrambi. Il programma crei dei processi che agiscano da lettori e scrittori utilizzando un unico buffer di memoria in cui sono memorizzati valori di tipo long. Il buffer di memoria è creato attraverso una shared memory e la sincronizzazione tra produttori e consumatori deve avvenire tramite l'utilizzo di semafori.*

Per risolvere tale problema, si utilizza una variable condivisa `numlettori` per contare il numero di lettori che contemporaneamente accedono alla risorsa. Solo quando nessun lettore è in attesa di leggere (`numlettori == 0`), gli scrittori potranno accedere (sempre uno alla volta) alla risorsa condivisa per poter scrivere. Rispetto alla soluzione con starvation dei soli scrittori, anche per gli scrittori si può ottenere un comportamento analogo ai lettori, introducendo una variabile `numscrittori`.

In questo scenario occorrono 4 semafori, tutti inizializzati a `1`:



- `MUTEX_NUMLETTORI` usato per gestire l'accesso alla variabile numlettori in mutua esclusione da parte dei lettori;

- `MUTEX_NUMSCRITTORI` per gestire l’accesso alla variabile numscrittori in mutua esclusione da parte degli scrittori

- `MUTEX_SCRITTORI` per gestire l’accesso in mutua esclusione alla risorsa condivisa da parte degli scrittori;

- `MUTEX_LETTORI_SCRITTORI` per garantire la mutua esclusione tra i processi lettori e scrittori.

Le operazioni di lettura sono sempre "protette" dalle procedure di `Inizio_Lettura()` e `Fine_Lettura()`, mentre le operazioni di scrittura sono sempre "protette" da `Inizio_Scrittura()` e `Fine_Scrittura()`.

Questa particolare strategia di sincronizzazione può tuttavia può provocare condizioni di attesa indefinita (starvation) per sia per i processi lettori che per i processi scrittori.