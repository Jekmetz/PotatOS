// This files serves as a central repository of the printable
// blocks of commands.c

// Below is the verions information
#define VERSION serial_println(\
      "Version 1.0"\
);

#define VERSION_FULL serial_println(\
        "Version 1.0\n"\
        "Module one\n"\
        "Developers:\n"\
        "\tHasan Ibraheem\n"\
        "\tHenry Vos\n"\
        "\tJay Kmetz\n"\
        "\tNicholas Fryer"\
);

// Below are all the help outputs
#define HELP serial_println(\
      "You can request a help page for the following commands"\
      " using help <cmd name>\n"\
      "help\n"\
      "version\n"\
      "shutdown\n"\
      "date\n"\
      "time"\
);

#define HELP_HELP serial_println(\
  "View help pages for individual commands."\
  " Use flag [-f | --full] for more information"\
);

#define HELP_HELP_FULL serial_println(\
          "Implementation:\n"\
          "\thelp <command> [-f | --full]\n"\
          "\n"\
          "Flags:\n"\
          "\t[-f | --full] - Show implementation and flags for each command\n"\
          "\t\twith explanations"\
);

#define HELP_VERSION serial_println(\
      "Display version information."\
      "Use flag [-f | --full] for more information"\
);

#define HELP_VERSION_FULL serial_println(\
        "Implementatoin:\n"\
        "\tversion [-f | --full]\n"\
        "Flags:\n"\
        "\t[-f | --full] - Show entire verion"\
);

#define HELP_SHUTDOWN serial_println(\
      "Shutdown the MPX System."\
      "Use flag [-f | --full] for more information"\
);

#define HELP_SHUTDOWN_FULL serial_println(\
        "Implementatoin:\n"\
        "\tshutdown\n"\
        "Flags:\n"\
        "\tNone\n"\
        "Notes:\n"\
        "\tMust confirm before shutdown"\
);

#define HELP_DATE serial_println(\
      "Display date."\
      "Use flag [-f | --full] for more information"\
);

#define HELP_DATE_FULL serial_println(\
        "Implementatoin:\n"\
        "\tdate [-s | --set]\n"\
        "Flags:\n"\
        "\t[-s | --set] - Set the date in DD-MM-YYYY\n"\
        "\t\tWhere all values are integers\n"\
        "Example:\n"\
        "\tdate -s 08-24-1994\n"\
        "\tdate -s 01-01-2019"\
);

#define HELP_TIME serial_println(\
      "Display time."\
      "Use flag [-f | --full] for more information"\
);

#define HELP_TIME_FULL serial_println(\
        "Implementatoin:\n"\
        "\tdate [-f | --full] [-s | --set]\n"\
        "Flags:\n"\
        "\t[-f | --full] - Show implementation and flags for each command\n"\
        "\t\twith explanations"\
        "\t[-s | --set] - Set the time in HH:MM:SS\n"\
        "\t\tWhere all values are integers and using 24 hour time\n"\
        "Example:\n"\
        "\time -s 12:24:32\n"\
        "\time -s 16:02:00"\
);
