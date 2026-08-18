//Programma che forca un argomento figlio e passa un argomento ad esso
#include <sys/types.h> //Libreria per i tipi di sistema come pid_t
#include <sys/wait.h> //Per la system call waitpid()
#include <stdlib.h> //Per exit()
#include <unistd.h> //Per fork(), execlp(), read(), write() e STDOUT/STDIN
#include <stdio.h> //Per sscanf() e perror()
#define BUFFSIZE 8192 //Buffer di  memoria di 8KB per contenere l'input digitato dall'utente

int main(void) {
    int n, status;
    pid_t pid;
    char buff[BUFFSIZE], commandname[20];
    n=write(STDOUT_FILENO, "\ninput command: ", 17); //Scrittura del prompt a schermo a basso livello -> STDOUT_FILENo è un descrittore del file ed indentifica il terminale
    n=read(STDIN_FILENO, buff,  BUFFSIZE);  //Legge ciò che l'utente digita dalla tastiera e lo salva nel buffer buff, infine ritorna il numero di caratteri letti n
    buff[n-1]=0; //La read memorizza anche il tasto(\n) alla fine. Questa riga sostituisce l'invio con il terminatore di stringa \0(ASCII 0)
    sscanf(buff, "%s", commandname); //Estrae solo la prima  parola del buffer e la salva nel commandname
    if(( pid = fork()) < 0) 
        perror("fork error"); //Debug per errore nella fork()
    else if(pid == 0) //Sono nel processo figlio
    if (execlp(commandname,buff,(char*)0) < 0) { //Cerca il programma commandname(es. ls) nella cartella di sistema (p cerca nel $PATH).
        perror("execlp error"); //Sostituisce l'immagine del processo figlio con il nuovo programma gli passa buff come argomento e termina la lista degli argomenti con un puntatore nullo
        exit(1); //Debug nel caso di insuccesso della execlp()
    }
    if((pid = waitpid(pid, &status, 0)) < 0)
    perror("wait error");
    n = write(STDOUT_FILENO, "\nDone!\n", 7);
    exit(0);
}
