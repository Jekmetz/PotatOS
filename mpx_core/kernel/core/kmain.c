/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <core/interrupts.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include "modules/mpx_supt.h"
#include "modules/cmdinput/command_handler.h"
#include "modules/pcb/pcb_wrangler.h"

void kmain(void) {
  extern uint32_t magic;
  // Uncomment if you want to access the multiboot header
  // extern void *mbd;
  // char *boot_loader_name = (char*)((long*)mbd)[16];

  // 0) Initialize Serial I/O and call mpx_init
  init_serial(COM1);
  set_serial_out(COM1);
  set_serial_in(COM1);

  klogv("Starting MPX boot sequence...");
  klogv("Initialized serial I/O on COM1 device...");

  // 1) Initialize the support software by identifying the current
  //     MPX Module.  This will change with each module.
  mpx_init(MODULE_R4);

  // 2) Check that the boot was successful and correct when using grub
  // Comment this when booting the kernel directly using QEMU, etc.
  if (magic != 0x2BADB002) {
    // kpanic("Boot was not error free. Halting.");
  }

  // 3) Descriptor Tables
  klogv("Initializing descriptor tables...");

  init_gdt();
  init_idt();

  // 4) Virtual Memory
  klogv("Initializing virtual memory...");

  init_pic();
  init_irq();
  sti();

  init_paging();

  klogv("Initializing process queues");
  init_process_queues();

  // 5) Add command handler and idle to ready queue
  klogv("Adding commhand to ready queue...");
  process_loader("commhand", 314159265, &command_handler);
  process_loader("idle", 314159266, &idle);
  asm volatile ("int $60");

  // 6) System Shutdown on return from your command handler
  klogv("Starting system shutdown procedure...");

  /* Shutdown Procedure */
  klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
  hlt();
}
