CC = gcc
CFLAGS = -pthread
TARGET = monitor_aria

OBJS = main.o lettori.o scrittori.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

main.o: main.c monitor_aria.h
	$(CC) $(CFLAGS) -c main.c

lettori.o: lettori.c monitor_aria.h
	$(CC) $(CFLAGS) -c lettori.c

scrittori.o: scrittori.c monitor_aria.h
	$(CC) $(CFLAGS) -c scrittori.c

clean:
	rm -f $(OBJS) $(TARGET)