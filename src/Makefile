CFLAGS=-g -Wall -Wextra -pedantic -std=c99
LDFLAGS=-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image
CC=gcc

all:
	$(CC) -c list.c -o list.o $(CFLAGS)
	$(CC) -c file.c -o file.o  $(CFLAGS) 
	$(CC) -c logic.c -o logic.o $(CFLAGS)
	$(CC) -c sdl.c -o sdl.o  $(CFLAGS)
	$(CC) -c main.c -o main.o $(CFLAGS)
	$(CC) *.o -o blackjack $(CFLAGS) $(LDFLAGS)
