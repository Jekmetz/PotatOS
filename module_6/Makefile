#
# Bad Makefile for Module 6

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -I.
DEPS = 
OBJ =\
module6.o\
commands.o\
command_utils.o

all: module6

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

module6: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) module6
