all: main lettore scrittore

main: main.o 
	gcc -o main main.o 

lettore: lettore.o procedure.o semafori.o
	gcc -o lettore lettore.o procedure.o semafori.o

scrittore: scrittore.o procedure.o semafori.o
	gcc -o scrittore scrittore.o procedure.o semafori.o

main.o: main.c header.h
	gcc -c main.c

lettore.o: lettore.c header.h
	gcc -c lettore.c

scrittore.o: scrittore.c header.h 
	gcc -c scrittore.c 

procedure.o: procedure.c header.h semafori.h 
	gcc -c procedure.c

semafori.o: semafori.c semafori.h 
	gcc -c semafori.c

clean: 
	rm -f *.o
	rm -f main scrittore lettore