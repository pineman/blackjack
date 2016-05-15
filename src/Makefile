CFLAGS=-g -Wall -Wextra -pedantic -std=c99
CPPFLAGS=-MP -MMD
LDFLAGS=-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image
CC=gcc
SRC=$(wildcard *.c)
EXEC=blackjack

all:
	make $(EXEC)
	rm -rf *.d

$(EXEC): $(SRC:%.c=%.o)
	$(CC) $(LDFLAGS) -o $@ $^

-include $(SRC:%.c=/%.d)
