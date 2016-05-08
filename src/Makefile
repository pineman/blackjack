CFLAGS=-g -Wall -Wextra -pedantic -std=c99
LDFLAGS=-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image
CC=gcc

all:
	$(CC) $(CFLAGS) -c list.c -o list.o
	$(CC) $(CFLAGS) -c file.c -o file.o
	$(CC) $(CFLAGS) -c logic.c -o logic.o
	$(CC) $(CFLAGS) -c sdl.c -o sdl.o
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) *.o -o blackjack
