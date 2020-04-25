/**
 * @file module6.c
 *
 * @brief Entry point for file system
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "command_utils.h"
#include "commands.h"
#include "file_wrangler.h"

ENTRY* file_recurse(BYTE* dir, BYTE* whole, const char* key);

int main(int argc, char** argv) {
  if (!(argc == 2 || argc == 3)) {
    printf("Usage: %s DISKIMAGE [PATH]\n", argv[0]);
    return 1;
  }

  if (access(argv[1], F_OK) == -1) {
    printf("Error: Diskimage file '%s' could not be found.\n", argv[1]);
    return 1;
  }

  char* filesystem_path = argv[2];
  // Load entire disk image into array
  // make functions for dealing with accessing disk image
  // Do things in commands

  loadEntireSystem(argv[1]);

  if (argc == 2)  // Command input mode
  {
    char command_buffer[512] = {0};
    // We will never need 16 arguments. Maybe we should just malloc.
    char* command_argv[16] = {0};

    while (1) {
      printf("\033[96mstdUser@potatOSR6:%s\033[0m > ", getCwdPath());

      // This happens when someone hits CTRL+D to send EOF
      if (fgets(command_buffer, 511, stdin) == NULL) {
        exit_command(0, NULL);
        break;
      }

      char* command = trim_whitespace(command_buffer);

      uint32_t command_argc = split_args(command, command_argv);

      if (command_argc == 0) {
        continue;
      }

      // From this pouint32_t we can just write programs almost exactly like you
      // normally would write desktop cli apps and we just call their function
      // with argc and argv

      command_t* found_command = search_commands(command_argv[0]);
      if (found_command == NULL) {
        printf("Command not found: %s\n", command_argv[0]);
      } else {
        found_command->function(command_argc, command_argv);
      }

      memset(command_argv, 0, sizeof(char*) * 16);
      memset(command_buffer, 0, 512);

      printf("\n");
    }
  } else if (argc == 3)  // file input mode
  {
    // recursively go through files and pruint out
    // info from found file.
    // otherwise, error report
    BYTE* whole = getSystem();
    ENTRY* file = file_recurse(getCWD(), whole, argv[2]);
    if (file == NULL) {
      printf("Could not locate file \"%s\"\n", argv[2]);
    } else {
      uint16_t* FAT = getDiabetes1();

      char entireFile[file->fileSize + SECTORSIZE];
      // We have found a match, bois.
      uint16_t fileClust = file->firstLogicalCluster;
      uint32_t count = 0;
      while (count < file->fileSize && fileClust < 0xFF8 &&
             FAT[fileClust] != 0) {
        memcpy((entireFile + count), whole + (fileClust + 31) * SECTORSIZE,
               SECTORSIZE);
        fileClust = FAT[fileClust];
        count += SECTORSIZE;
      }

      // Terminal Setup for Any Key
      struct termios old, new;
      tcgetattr(0, &old);
      tcgetattr(0, &new);     /* get current terminal attirbutes; 0 is the file
                                 descriptor for stdin */
      new.c_lflag &= ~ICANON; /* disable canonical mode */
      new.c_cc[VMIN] = 1;     /* wait until at least one keystroke available */
      new.c_cc[VTIME] = 0;    /* no timeout */
      tcsetattr(0, TCSANOW, &new); /* set immediately */

      // print entire boi 25 lines at a time
      uint32_t lineCount = 0;
      char c, recieved;
      for (uint32_t chi = 0; chi < file->fileSize; chi++) {
        c = entireFile[chi];
        if (c == '\n')
          lineCount++;
        putc(c, stdout);

        if (lineCount == 25) {
          printf("------- PRESS ANY KEY (q to quit) -------\r");
          lineCount = 0;
          recieved = getchar();

          if (recieved == '\n') {
            printf("\033[1A");
            printf("                                         \r");
            printf("\033[1A");
          } else if (recieved == 'q') {  // they doneski bois
            printf("\b");
            printf("                                         \r");
            printf("\033[1A");
            break;
          } else {
            printf("\b");
            printf("                                         \r");
            printf("\033[1A");
          }
        }
      }

      tcsetattr(0, TCSANOW, &old); /* set immediately */
    }
  } else {
    puts("Invalid arguments");
  }

  return 0;
}

ENTRY* file_recurse(BYTE* dir, BYTE* whole, const char* key) {
  // getting dir info
  uint32_t num_entries = *((uint32_t*)dir);
  dir += sizeof(uint32_t);
  uint32_t sector = *((uint32_t*)dir);
  dir += sizeof(uint32_t);

  ENTRY* cur;
  for (uint32_t i = 0; i < num_entries; i++) {
    cur = (ENTRY*)(dir + i * (sizeof(ENTRY)));
    if (cur->empty == 0) {
      char filen[32] = {0};
      strcpy(filen, cur->fileName);
      strcat(filen, ".");
      strcat(filen, cur->extension);
      if (strcasecmp(filen, key) == 0) {
        return cur;
      } else if (strcasecmp(cur->extension, "   ") == 0 &&
                 strstr(cur->fileName, ".") != (cur->fileName)) {
        BYTE* sub_dir = NULL;
        loadDir(&sub_dir, whole, 33 + cur->firstLogicalCluster - 2);
        ENTRY* sub_ent = file_recurse(sub_dir, whole, key);
        if (sub_ent != NULL) {
          return sub_ent;
        }
      }
    }
  }
  return NULL;
}