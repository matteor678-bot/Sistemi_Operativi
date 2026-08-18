# Esercizio 1
In questo esercizio, si implementa una comunicazione inter-processo utilizzando una shared memory, disciplinando l'accesso con un semaforo. In particolare, si vuole creare 2 processi figli che incrementano per 100 volte una variabile intera memorizzata nella shared memory.

Nei file [semafori.h](semafori.h) e [semafori.c](semafori.c) sono definite e implementate le operazioni di wait e signal su semaforo.

Nel file [main.c](main.c) è implementato il *main* del programma richiesto. 

Alcune domande:

- Cosa succederebbe se eliminassi la chiamata a `Wait_Sem()`o a `Signal_Sem()`o a entrambi?
1. Se elimino la Wait_Sem() provoco una race condition.
2. Se elimino la Signal_Sem() provoco un deadlock(situazione di stallo), poichè entrambi i programmi si bloccherebbero per un tempo indefinito.
3. Se tolgo entrambe le funzioni ritorna ad esserci una race condition come nel caso 1., perchè sto eliminando tutti i meccanismi di sicronizzazione.

- Cosa indica il secondo paramentro della semget() ? 
1. Il secondo parametro della `semget()` indica la dimensione dell'array di semafori da istanziare.

- Cosa cambia utilizzando i flag `IPC_CREAT | IPC_EXCL` per la `shmget()` e la `semget()` ?
1. La due operazioni hanno successo solo nel caso non sia stata creata una IPC con la stessa chiave.