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


