CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -m64 `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sqlite3`
LDFLAGS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sqlite3`

all: main

main: main.o function.o gui.o db.o
	$(CC) $(CFLAGS) -o main main.o function.o gui.o db.o $(LDFLAGS)

main.o: main.c function.h
	$(CC) $(CFLAGS) -c main.c

function.o: function.c function.h
	$(CC) $(CFLAGS) -c function.c

gui.o: gui.c function.h
	$(CC) $(CFLAGS) -c gui.c

db.o: db.c db.h
	$(CC) $(CFLAGS) -c db.c

clean:
	rm -f main *.o
