#
# Makefile for MPX/Linux

AS      = nasm
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c -m32
LD      = ld
LDFLAGS = -m elf_i386
ASFLAGS = -f elf32 -g

OBJFILES =\
core/gdt.o\
core/idt.o\
core/interrupts.o\
core/io.o\
core/stdio.o\
core/irq.o\
core/kmain.o\
core/serial.o\
core/system.o\
core/tables.o\
core/utility.o\
mem/paging.o\
mem/heap.o

.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -I../ -o $@ $<

all: kernel.o

kernel.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r -o kernel.o $(OBJFILES)

clean:
	rm $(OBJFILES)
