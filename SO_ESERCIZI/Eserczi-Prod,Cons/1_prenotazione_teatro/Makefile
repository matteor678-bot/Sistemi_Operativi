all: clienti visualizzatore

visualizzatore: procedure.o semafori.o visualizzatore.o
	gcc -o visualizzatore procedure.o semafori.o visualizzatore.o

clienti: procedure.o semafori.o clienti.o
	gcc -o clienti procedure.o semafori.o clienti.o

clienti.o: clienti.c procedure.h
	gcc -c clienti.c 

visualizzatore.o: visualizzatore.c procedure.h
	gcc -c visualizzatore.c 

procedure.o: procedure.c procedure.h
	gcc -c procedure.c 

semafori.o: semafori.c semafori.h 
	gcc -c semafori.c 

clean:
	rm -rf *.o
	rm -rf clienti visualizzatore