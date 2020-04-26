/**
 * @file commands.c
 *
 * @brief Holds all command functions for FAT12 Emulation
 */
#include "commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <termios.h>

#include "file_wrangler.h"

static command_t COMMANDS[] = {
    {
        .command_name = "pbsi",
        .function = pbsi_command,
        .command_usage = "pbsi",
        .command_description = "Prints the boot sector information.",
        .command_examples = "",
    },
    {
        .command_name = "prd",
        .function = prd_command,
        .command_usage = "prd",
        .command_description =
            "Prints the root directory entries and their attributes.",
        .command_examples = "",
    },
    {
        .command_name = "root",
        .function = root_command,
        .command_usage = "root",
        .command_description =
            "Changes the current directory to be the root directory.",
        .command_examples = "",
    },
    {
        .command_name = "cd",
        .function = cd_command,
        .command_usage = "cd DIR",
        .command_description = "Changes the current directory.",
        .command_examples = "",
    },
    {
        .command_name = "ls",
        .function = ls_command,
        .command_usage = "ls [DIR]",
        .command_description = "List the entries in a directory.",
        .command_examples = "ls files",
    },
    {
        .command_name = "type",
        .function = type_command,
        .command_usage = "type FILE",
        .command_description = "Print the contents of a TXT, BAT, or C file.",
        .command_examples = "type 1984.txt",
    },
    {
        .command_name = "rename",
        .function = rename_command,
        .command_usage = "rename OLD_FILE NEW_FILE",
        .command_description = "Rename a file in the current directory.",
        .command_examples = "rename a.txt b.txt",
    },
    {
        .command_name = "move",
        .function = move_command,
        .command_usage = "move FILE NEW_DIRECTORY",
        .command_description = "Move a file to a new directory.",
        .command_examples = "move a.txt /\nmove a.txt files\n",
    },
    {
        .command_name = "help",
        .function = help_command,
        .command_usage = "help [COMMAND]",
        .command_description = "Get help for commands.",
        .command_examples = "help\nhelp ls\nhelp help help\n",
    },
    {
        .command_name = "exit",
        .function = exit_command,
        .command_usage = "exit",
        .command_description = "Exits this program",
        .command_examples = "",
    },
    {0}};

command_t* search_commands(char* command) {
  command_t* current_command = COMMANDS;

  while (current_command->command_name != NULL) {
    if (strcmp(command, current_command->command_name) == 0) {
      return current_command;
    }

    current_command += 1;
  }

  return NULL;
}

int help_command(int argc, char** argv) {
  command_t* current_command = COMMANDS;

  if (argc > 2) {
    bool call_911 = true;

    for (int arg_i = 0; arg_i < argc; arg_i++) {
      if (strcmp(argv[arg_i], "help") != 0) {
        call_911 = false;
        break;
      }
    }

    if (call_911) {
      printf("Calling 911...\n");
    } else {
      printf("Too many arguments given, see `help help`\n");
    }

    return 0;
  }

  if (argc == 2) {
    char* command = argv[1];

    while (current_command->command_name != NULL) {
      if (strcmp(command, current_command->command_name) == 0) {
        printf("Description:\n   %s\n\n", current_command->command_description);
        printf("Usage:\n   %s\n", current_command->command_usage);

        char* examples = current_command->command_examples;
        int ex_len = strlen(examples);

        if (ex_len > 0) {
          printf("\nExamples:\n");

          while (*examples != '\0') {
            char* found_newline = strchr(examples, '\n');

            if (found_newline == NULL) {
              found_newline = examples + ex_len;
            }

            int index = (int)(found_newline - examples);

            printf(" - %.*s\n", index, examples);

            examples = examples + index;

            if (*examples == '\n') {
              examples += 1;
            }
          }
        }

        return 0;
      }

      current_command += 1;
    }

    printf("Could not find command: %s\n", command);
  }

  printf("Available commands:\n");

  while (current_command->command_name != NULL) {
    printf(" - %s\n", current_command->command_name);

    current_command += 1;
  }

  return 0;
}

int pbsi_command(int argc, char** argv) {
  BOOTSECTORSTRUCT* bootSectorIn = getBootSectorIn();
  printf("Bytes per sector: %d\n", bootSectorIn->sectorsPerCluster);
  printf("Sectors per cluster: %d\n", bootSectorIn->bytesPerSector);
  printf("Number of reserved sectors: %d\n", bootSectorIn->numReservedSectors);
  printf("Number of FAT copies: %d\n", bootSectorIn->numFATCopies);
  printf("Max number of root directory entries: %d\n",
         bootSectorIn->maxNumRoot);
  printf("Total # of sectors in the file system: %d\n",
         bootSectorIn->numOfSectors);
  printf("Number of sectors per FAT: %d\n", bootSectorIn->numSECTORSPERFAT);
  printf("Sectors per track: %d\n", bootSectorIn->sectorsPerTrack);
  printf("Number of heads: %d\n", bootSectorIn->numberHeads);
  printf("Total sector count for FAT32: %d\n", bootSectorIn->sectorCountFAT32);
  printf("Boot signature: %02X\n", bootSectorIn->bootSig);
  printf("Volume ID: %d\n", bootSectorIn->volumeID);
  printf("Volume label is: %s\n", bootSectorIn->volumeLabel);
  printf("File System Type: %s\n", bootSectorIn->fileSystemType);
  return 0;
}

/**
 * @brief print root directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int prd_command(int argc, char** argv) {
  // Grab the CWD
  BYTE* cwdIn = getRoot();

  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  ENTRY* cur;
  for (uint32_t i = 0; i < numEntries; i++) {
    cur = (ENTRY*)(cwdIn + i * sizeof(ENTRY));
    if (cur->empty == 0) {
      if (cur->attributes != 0x0F && !(cur->attributes & 0x02) &&
          !(cur->attributes & 0x08)) {
        printf(
            "Filename: %s\n"
            "Extension: %s\n"
            "Attributes: %x\n"
            "Creation Time: %hu\n"
            "Creation Date: %hu\n"
            "Last Access Date: %hu\n"
            "Last Write Date: %hu\n"
            "First Locical Cluster: %x\n"
            "File Size: %d\n\n",
            cur->fileName, cur->extension, cur->attributes, cur->creationTime,
            cur->creationDate, cur->lastAccessDate, cur->lastWriteDate,
            cur->firstLogicalCluster, cur->fileSize);
      }
    }
  }
  return 0;
}

/**
 * @brief Returns to the root directory.
 *
 * Sends user's current working directory to the root.
 *
 * @param argc Number of arguments
 * @param argv List of string arguments.
 * @return int Success condition.
 */
int root_command(int argc, char** argv) {
  BYTE* cwdIn = getCWD();

  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  BYTE* whole = getSystem();

  loadCWD(whole, 19);
  cwdIn = getCWD();
  setCwdPath(((ENTRY*)(cwdIn + 8))->fileName);

  return 0;
}

int cd_command(int argc, char** argv) {
  // If they didn't use it right
  if (argc != 2) {
    printf("Check your use of cd there, friendo! cd <directory>\n");
    return 1;
  }
  // If there is a dot position
  if (findDotPosition(argv[1]) && strcmp(argv[1], ".") != 0 &&
      strcmp(argv[1], "..") != 0) {
    printf("That's not a directory you dingus.\n");
    return 1;
  }
  // Grab the CWD
  BYTE* cwdIn = getCWD();

  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  BYTE* whole = getSystem();
  char* title;

  // int place;

  // ENTRY* cur;
  if (strcmp(argv[1], ".") == 0) {
  }  // Do nothing, covers cd . appending to CwdPath
  else if (argv[1] != NULL) {
    ENTRY* cur;
    for (uint32_t i = 0; i < numEntries; i++) {
      cur = (ENTRY*)(cwdIn + i * sizeof(ENTRY));
      if (cur->empty != 1 && strcasecmp(cur->fileName, argv[1]) == 0) {
        // printf("Yep: %s\tGo to raw: %d\n", cwdIn[i].fileName,
        // cwdIn[i].firstLogicalCluster);
        if (cur->firstLogicalCluster == 0x00) {
          loadCWD(whole, 19);
          cwdIn = getCWD();
          setCwdPath(((ENTRY*)(cwdIn + 8))->fileName);
        } else {
          char path[10];
          path[0] = '/';
          strcpy(path + 1, cur->fileName);
          printf("%s\n", path);
          setCwdPath(strcat(getCwdPath(), path));
          loadCWD(whole, 33 + cur->firstLogicalCluster - 2);
        }
      }
    }
  } else {
    printf("Have to specify dir to go to next\n");
  }

  return 0;
}

/**
 * @brief list directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int ls_command(int argc, char** argv) {
  // Grab the CWD
  BYTE* cwdIn = getCWD();

  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  // get a flag ready for if it is a directory
  BYTE isDir = 0;
  ENTRY* cur;

  uint8_t filenamelen;

  if (argc == 1) {  // if we are listing the directory...
    printf("\033[33m%-13s%-15s%-25s\033[0m\n","Name","File Size", "Starting Logical Cluster");
    for (uint32_t i = 0; i < numEntries; i++)  // for the number of entries...
    {
      cur = (ENTRY*)(cwdIn + i * sizeof(ENTRY));
      if (cur->empty != 1 && cur->attributes != 0x0F &&
          !(cur->attributes & 0x02) && !(cur->attributes & 0x08)) {
        // if it is not a long file name,
        // is not hidden,
        // and is not empty...
        isDir = cur->attributes & 0x10;
        if (isDir)  // directory
          printf("\033[32m%-13s%-15d%-25x\033[0m\n", cur->fileName, cur->fileSize, cur->firstLogicalCluster);
        else  // not directory
        {
          filenamelen = strlen(cur->fileName);
          printf("%-*s.%-*s%-15d%-25x\n", filenamelen, cur->fileName, 12 - filenamelen, cur->extension, cur->fileSize, cur->firstLogicalCluster);
        }
      }
    }
  } else if (argc == 2) {  // if we are printing a file...
    isDir = strchr(argv[1], '.') == NULL;
    char* fileName = argv[1];
    char curFileName[13];
    BYTE found = 0;
    for (uint32_t i = 0; i < numEntries; i++) {
      cur = (ENTRY*)(cwdIn + i * sizeof(ENTRY));
      if (cur->empty == 0) {
        // get curFileName
        curFileName[0] = '\0';
        strcat(curFileName, cur->fileName);
        if (!isDir)
          strcat(curFileName, ".\0");
        if (!isDir)
          strcat(curFileName, cur->extension);

        if (strcasecmp(fileName, curFileName) ==
            0) {  // If we have found a match...
          found = 1;
          printf(
              "Filename: %s\n"
              "Extension: %s\n"
              "Attributes: %x\n"
              "Creation Time: %hu\n"
              "Creation Date: %hu\n"
              "Last Access Date: %hu\n"
              "Last Write Date: %hu\n"
              "First Locical Cluster: %x\n"
              "File Size: %d\n\n",
              cur->fileName, cur->extension, cur->attributes, cur->creationTime,
              cur->creationDate, cur->lastAccessDate, cur->lastWriteDate,
              cur->firstLogicalCluster, cur->fileSize);
          break;
        }
      }
    }
    if (!found) {
      printf("No file found under  that name!\n");
    }
  } else {  // they typed the command wrong
    printf("Check your syntax there, boss! ls [<filename>]\n");
  }
  return 0;
}

/**
 * @brief prints out contents of file
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int type_command(int argc, char** argv) {
  // error checking!
  if (argc != 2) {
    printf(
        "Check your syntax there hoss! type <filename including extension>\n");
    return 1;
  } else if (strstr(argv[1], ".") == NULL) {
    printf("Must be a file including a '.' between the name and extension!\n");
    return 1;
  }

  BYTE* sys = getSystem();
  uint16_t* FAT = getDiabetes1();
  char* fileName = argv[1];
  char curFileName[13];

  BYTE* cwdIn = getCWD();
  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  BYTE found = 0;

  ENTRY* cur;
  for (uint32_t i = 0; i < numEntries; i++) {
    cur = (ENTRY*)(cwdIn + i * sizeof(ENTRY));
    if (cur->empty == 0) {
      // get curFileName
      curFileName[0] = '\0';
      strcat(curFileName, cur->fileName);
      strcat(curFileName, ".\0");
      strcat(curFileName, cur->extension);
      if (strcasecmp(fileName, curFileName) == 0) {
        found = 1;
        char entireFile[cur->fileSize + SECTORSIZE];
        // We have found a match, bois.
        uint16_t curClust = cur->firstLogicalCluster;
        uint32_t count = 0;
        while (count < cur->fileSize && curClust < 0xFF8 &&
               FAT[curClust] != 0) {
          memcpy((entireFile + count), sys + (curClust + 31) * SECTORSIZE,
                 SECTORSIZE);
          // fwrite(sys+(curClust+31)*512,512,1,stdout);
          // printf("------Press [SPACE]------");
          // if(argc > 2) getchar();
          // printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
          curClust = FAT[curClust];
          count += SECTORSIZE;
        }

        // Terminal Setup for Any Key
        struct termios old, new;
        tcgetattr(0, &old);
        tcgetattr(0, &new); /* get current terminal attirbutes; 0 is the file
                               descriptor for stdin */
        new.c_lflag &= ~ICANON; /* disable canonical mode */
        new.c_cc[VMIN] = 1;  /* wait until at least one keystroke available */
        new.c_cc[VTIME] = 0; /* no timeout */
        tcsetattr(0, TCSANOW, &new); /* set immediately */

        // print entire boi 25 lines at a time
        uint32_t lineCount = 0;
        char c, recieved;
        for (uint32_t chi = 0; chi < cur->fileSize; chi++) {
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
        break;
      }
    }
  }

  if (!found) {
    printf("That file was not found there my friend!\n");
  }

  return 0;
}

/**
 * @brief renames a file or directory
 *
 * @param argc count of arguments passed in
 * @param argv list of strings separated by space characters passed in
 *
 * @return 0
 */
int rename_command(int argc, char** argv) {
  // error checking
  if (argc != 3) {
    printf("Check your syntax there friendo! rename <oldname> <newname>\n");
    return 1;
  }

  BYTE* cwdIn = getCWD();                     // get cwdIn
  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  int16_t dotPosition = findDotPosition(argv[2]);
  char newFileName[8] = {0};
  char newExt[3] = {0};
  unsigned char hasExt = 0;

  if (dotPosition > 0) {
    if (dotPosition == 1) {
      printf("Enough with your dirty tricks, Hasan.\n");
      return 1;
    }
    if (strlen(argv[2]) - dotPosition <= 0) {
      printf("Make sure you have a vaild extension.\n");
      return 1;
    }
    if (dotPosition > 8) {
      printf("File name can be at most 8 characters!\n");
      return 1;
    }
    if (strlen(argv[2]) - dotPosition > 3) {
      printf("File extension can be at most 3 characters!\n");
      return 1;
    }
    // if(strlen(argv[2]) != (uint16_t)(dotPosition + 3))
    //{   //invalid extension
    //    printf("Extension must be 3 characters long!\n");
    //    return 1;
    //}
    // This has an extension
    hasExt = 1;
    memcpyUpper(newFileName, argv[2], 8);
    newFileName[dotPosition - 1] = '\0';
    memcpyUpper(newExt, argv[2] + dotPosition, 3);
  } else {
    // This does not have an extension
    hasExt = 0;
    memcpyUpper(newFileName, argv[2], 8);
  }
  BYTE found = 0;
  char curFileName[13];
  ENTRY* cur;

  for (uint32_t i = 0; i < numEntries; i++) {
    cur = (ENTRY*)(cwdIn + i * (sizeof(ENTRY)));

    if (cur->empty == 0) {
      // get curFileName
      curFileName[0] = '\0';
      strcat(curFileName, cur->fileName);
      if (hasExt)
        strcat(curFileName, ".\0");
      if (hasExt)
        strcat(curFileName, cur->extension);
      if (strcasecmp(argv[1], curFileName) == 0) {
        found = 1;
        BYTE* curEntryLoc = getSystem() + (curSector * SECTORSIZE) + (i * 32);
        // We have a match there parnter
        // FileName
        memcpy(curEntryLoc, newFileName, 8);
        if (hasExt) {
          memcpy(curEntryLoc + 8, newExt, 3);
        }
        if (hasExt) {
        }
        break;
      }
    }
  }

  if (!found) {
    printf("That file was not found!\n");
    return 1;
  } else {
    printf("Rename successful!\n");
  }

  // Load the new cwd if success!
  loadCWD(getSystem(), curSector);

  return 0;
}

/**
 * @brief Moves a file from one directory to another.
 * 
 * Takes the name of a file and directory within the same directory, and moves the file entry
 * into the specified directory.
 * 
 * @param argc Number of arguments.
 * @param argv List of arguments argv[1] is the file to move, argv[2] is the destination directory.
 * @return int Success condition.
 */
int move_command(int argc, char** argv) {
  if (argc < 3) {
    puts(
        "Not enough arguments, command syntax is\n\tmove <file> <destination "
        "dir>");
    return 1;
  }

  BYTE* cwdIn = getCWD();                     // get cwdIn
  uint32_t numEntries = *((uint32_t*)cwdIn);  // get the number of entries
  cwdIn += sizeof(uint32_t);
  uint32_t curSector = *((uint32_t*)cwdIn);
  cwdIn += sizeof(uint32_t);  // move cwdIn to the start of the entries

  ENTRY *file = NULL, *dir = NULL, *cur;
  BYTE* file_location;

  // finding directory and file
  for (uint32_t i = 0; i < numEntries; i++) {
    cur = (ENTRY*)(cwdIn + i * (sizeof(ENTRY)));
    char filen[32] = {0};
    strcpy(filen, cur->fileName);
    strcat(filen, ".");
    strcat(filen, cur->extension);
    if (cur->empty == 0) {
      if (strcasecmp(cur->extension, "   ") == 0 &&
          strcasecmp(cur->fileName, argv[2]) == 0) {
        dir = cur;
      } else if (strcasecmp(argv[1], filen) == 0) {
        file = cur;
        file_location = getSystem() + (curSector * SECTORSIZE) + (i * 32);
      }
    }
  }

  if (file == NULL) {
    printf("Could not locate file : \"%s\"\n", argv[1]);
  }
  if (dir == NULL) {
    printf("Could not locate directory : \"%s\"\n", argv[2]);
  }

  if (strcasecmp(argv[2], ".") == 0) {
    printf("Nothing to be done.");
    return 1;
  }

  // getting location of entry list of destination directory
  BYTE* dest = NULL;
  if (dir->firstLogicalCluster != 0x00) {
    loadDir(&dest, getSystem(), 33 + dir->firstLogicalCluster - 2);
  } else {
    loadDir(&dest, getSystem(), 19);
  }
  uint32_t* destNumEntries = ((uint32_t*)dest);
  dest += sizeof(uint32_t);  // going to the first entry
  uint32_t destStartSec = *((uint32_t*)dest);

  // searching for holes in the entry list
  for (uint32_t i = 0; i < *destNumEntries; i++) {
    cur = (ENTRY*)(dest + i * (sizeof(ENTRY)));
    if (cur->empty == 1) {
      // entry is empty and able to be used
      BYTE* newEntryLoc = getSystem() + (destStartSec * SECTORSIZE) + (i * 32);
      memcpy(newEntryLoc, &(file->fileName), 8);
      newEntryLoc += 8;
      memcpy(newEntryLoc, &(file->extension), 3);
      newEntryLoc += 3;
      memcpy(newEntryLoc, &(file->attributes), 19);
      newEntryLoc += 1;
      memcpy(newEntryLoc, &(file->reserved), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->creationTime), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->creationDate), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->lastAccessDate), 2);
      newEntryLoc += 4;
      memcpy(newEntryLoc, &(file->lastWriteTime), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->lastWriteDate), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->firstLogicalCluster), 2);
      newEntryLoc += 2;
      memcpy(newEntryLoc, &(file->fileSize), 4);
      file->empty = 1;
      *file_location = 0xE5;
      break;
    }
  }

  if (file->empty == 0) {
    // Have to add one to the end
    BYTE* newEntryLoc =
        getSystem() + (destStartSec * SECTORSIZE) + ((*destNumEntries) * 32);
    memcpy(newEntryLoc, &(file->fileName), 8);
    newEntryLoc += 8;
    memcpy(newEntryLoc, &(file->extension), 3);
    newEntryLoc += 3;
    memcpy(newEntryLoc, &(file->attributes), 19);
    newEntryLoc += 1;
    memcpy(newEntryLoc, &(file->reserved), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->creationTime), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->creationDate), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->lastAccessDate), 2);
    newEntryLoc += 4;
    memcpy(newEntryLoc, &(file->lastWriteTime), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->lastWriteDate), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->firstLogicalCluster), 2);
    newEntryLoc += 2;
    memcpy(newEntryLoc, &(file->fileSize), 4);

    (*destNumEntries)++;
    file->empty = 1;  // this is only a more immediate solution to remove
    *file_location = 0xE5;
  }

  return 0;
}

/**
 * @brief Exits the shell.
 * 
 * Confirms user wants to exit, and saves the state of the image.
 * 
 * @param argc Number of arguments
 * @param argv List of arguments. (None are applicable).
 * @return int Success condition.
 */
int exit_command(int argc, char** argv) {
  puts("\n\033[33;43m▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\033[0m");
  puts(
      "\033[33;43m▓\033[0;33m   Warning system shutting down!   "
      "\033[33;43m▓\033[0m");
  puts("\033[33;43m▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\033[0m");
  printf("\nAre you sure that you want to exit?: ");

  char conf[4] = {'\0'};
  fgets(conf, 4, stdin);

  if (conf[0] == 'y' || conf[0] == 'Y') {
    FILE* fp = fopen(getImagePath(), "w");
    fwrite(getSystem(), getSystemSize(), 1, fp);
    fclose(fp);
    puts(
        "\nGoodbye and have a "
        "\033[1;31ms\033[33mp\033[32ml\033[36me\033[34mn\033[35md\033[31mi\033["
        "33mf\033[32me\033[36mr\033[34mo\033[35mu\033[31ms\033[0m day!\n");
    exit(0);
  }

  return 0;
}
