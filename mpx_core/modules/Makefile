#
# Makefile for MPX Modules

AS      = nasm
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c -m32
LD      = ld
LDFLAGS = -m elf_i386
ASFLAGS = -f elf32 -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o\
cmdinput/command_handler.o\
cmdinput/poll_input.o\
cmdinput/splash.o\
commands/cmdHelp.o\
commands/commandUtils.o\
commands/time.o\
commands/cmdClear.o\
commands/cmdDate.o\
commands/cmdTime.o\
commands/cmdVersion.o\
commands/cmdBlockPCB.o\
commands/cmdCreatePCB.o\
commands/cmdDeletePCB.o\
commands/cmdResume.o\
commands/cmdSetPriority.o\
commands/cmdShowAllPCBs.o\
commands/cmdShowBlockedPCBs.o\
commands/cmdShowPCB.o\
commands/cmdShowReadyPCBs.o\
commands/cmdShowSusReadyPCBs.o\
commands/cmdSuspend.o\
commands/cmdUnblockPCB.o\
commands/cmdPotat.o\
commands/cmdLoadr3.o\
commands/cmdYield.o\
commands/procsr3.o\
commands/cmdInfinity.o\
pcb/pcb_queue.o\
commands/cmdTestPCB.o\
pcb/pcb_wrangler.o\
pcb/pcb_utils.o\
commands/cmdAlias.o\
commands/cmdAlarm.o\
alarms/alarmWrangler.o\
commands/cmdHistory.o\
memory_managment/memory_wrangler.o

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r -o modules.o $(OBJFILES)

clean:
	rm modules.o $(OBJFILES)
