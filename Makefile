CC=gcc
CFLAGS=-std=c99 -Werror -Wvla -pedantic -Wextra -Wall

SRCS=src/accept.c src/check.c src/init.c src/minifim.c src/update.c \
	 src/cli.c src/strings_utils.c
OBJS=$(SRCS:.c=.o)
BIN=minifim

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -fsanitize=address -g3

clean:
	rm -f $(BIN) $(OBJS)

.PHONY: clean all debug
