#CFLAGS=-g -Wall -Wextra -pedantic -std=c99 $(shell sdl2-config --cflags)
CFLAGS=-O3 -Wall -Wextra -pedantic -std=c99 $(shell sdl2-config --cflags)
CPPFLAGS=-MP -MMD
LDFLAGS=-lm $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image
CC=gcc
SRC=$(wildcard *.c)
EXEC=blackjack

all:
	make $(EXEC)
	rm -rf *.d

$(EXEC): $(SRC:%.c=%.o)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o
	rm blackjack

-include $(SRC:%.c=/%.d)
