all: lett_scritt_starv_entrambi

lett_scritt_starv_entrambi: lett_scritt_starv_entrambi.o procedure.o semafori.o
	gcc -o lett_scritt_starv_entrambi lett_scritt_starv_entrambi.o procedure.o semafori.o

lett_scritt_starv_entrambi.o: lett_scritt_starv_entrambi.c header.h
	gcc -c lett_scritt_starv_entrambi.c

procedure.o: procedure.c header.h 
	gcc -c procedure.c

semafori.o: semafori.c semafori.h 
	gcc -c semafori.c

clean:
	rm -f *.o
	rm -f lett_scritt_starv_entrambi