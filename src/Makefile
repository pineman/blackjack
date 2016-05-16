CFLAGS=-g -Wall -Wextra -pedantic -std=c99
CPPFLAGS=-MP -MMD
LDFLAGS=$(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image
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

win32:
	make $(EXEC)_win32
	rm -rf *.d

$(EXEC)_win32: $(SRC:%.c=%.o)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

-include $(SRC:%.c=/%.d)
