#pragma once
#include <core/stdio.h>
// This files serves as a central repository of the printable
// blocks of commands.c


// Below is the verions information
#define VERSION puts(\
      "Version 1.0"\
);

#define VERSION_FULL puts(\
        "Version 1.0\n"\
        "Module one\n"\
        "Developers:\n"\
          "\tHasan Ibraheem\n"\
          "\tHenry Vos\n"\
          "\tJay Kmetz\n"\
          "\tNicholas Fryer"\
);

// Below are all the help outputs
#define HELP puts(\
      "You can request a help page for the following commands"\
      " using help [-c | --cmd] <cmd name>\n"\
        "\thelp\n"\
        "\tversion\n"\
        "\tshutdown\n"\
        "\tdate\n"\
        "\ttime"\
);

#define HELP_HELP puts(\
  "View help pages for individual commands."\
  " Use flag [-f | --full] for more information"\
);

#define HELP_HELP_FULL puts(\
          "Usage:\n"\
            "\thelp [-c | --cmd] <command> [-f | --full]\n"\
          "\n"\
          "Flags:\n"\
            "\t[-c | --cmd] - Show the help page for a certain command\n"\
            "\t[-f | --full] - Show implementation and flags for each command\n"\
            "\t\twith explanations"\
);

#define HELP_VERSION puts(\
      "Display version information."\
      "Use flag [-f | --full] for more information"\
);

#define HELP_VERSION_FULL puts(\
        "Usage:\n"\
          "\tversion [-f | --full]\n"\
        "Flags:\n"\
          "\t[-f | --full] - Show entire verion"\
);

#define HELP_SHUTDOWN puts(\
      "Shutdown the POS System."\
      " Use flag [-f | --full] for more information"\
);

#define HELP_SHUTDOWN_FULL puts(\
        "Usage:\n"\
        "\tshutdown\n"\
        "Flags:\n"\
        "\tNone\n"\
        "Notes:\n"\
        "\tMust confirm with Yes before shutdown"\
);

#define HELP_DATE puts(\
      "Display date."\
      " Use flag [-f | --full] for more information"\
);

#define HELP_DATE_FULL puts(\
        "Usage:\n"\
        "\tdate [-s | --set]\n"\
        "Flags:\n"\
        "\t[-s | --set] - Set the date in DD/MM/YYYY\n"\
        "\t\tWhere all values are integers\n"\
        "Example:\n"\
        "\tdate -s 08/24/1994\n"\
        "\tdate --set 01/01/2019"\
);

#define HELP_TIME puts(\
      "Display time."\
      " Use flag [-f | --full] for more information"\
);

#define HELP_TIME_FULL puts(\
        "Usage:\n"\
          "\tdate [-s | --set]\n"\
        "Flags:\n"\
          "\t[-s | --set] - Set the time in HH:MM:SS\n"\
          "\t\tWhere all values are integers and using 24 hour time\n"\
        "Example:\n"\
          "\ttime -s 12:24:32\n"\
          "\ttime --set 16:02:00"\
);
