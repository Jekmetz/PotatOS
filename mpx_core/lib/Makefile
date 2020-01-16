#
# Makefile for MPX/Linux libs

AS      = nasm
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c -m32
LD      = ld
LDFLAGS = -m elf_i386
ASFLAGS = -f elf32 -g

OBJFILES =\
string.o

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -c -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: lib.o

lib.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r -o lib.o $(OBJFILES)

clean:
	rm $(OBJFILES)
