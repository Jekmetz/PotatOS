#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "command_utils.h"

char *trim_whitespace(char *str) {
    while (isspace(*str)) {
        *str = 0;
        str += 1;
    }

    char *end = str + strlen(str) - 1;

    while (isspace(*end)) {
        *end = 0;
        end -= 1;
    }

    return str;
}

uint32_t split_args(char *command, char **argv) {
    // Delegate the responsibility of making an array of char pointers to the calling function
    uint32_t argc = 0;
    uint32_t argv_index = 0;

    bool last_char_was_space = true;

    uint32_t command_i = 0;
    bool keep_going = true;

    while (keep_going) {
        switch (command[command_i]) {
            case ' ':
                command[command_i] = 0;

                if (!last_char_was_space) {
                    argc += 1;
                    last_char_was_space = true;
                }
                break;

            case 0:
                if (!last_char_was_space) {
                    argc += 1;
                    argv[argv_index] = command + command_i;
                }
                keep_going = false;
                break;

            default:
                if (last_char_was_space) {
                    last_char_was_space = false;
                    argv[argv_index] = command + command_i;
                    argv_index += 1;
                }
                break;
        }

        command_i += 1;
    }

    return argc;
}

void memcpyUpper(char* dest, const char* source, int nchars)
{
    for(int i = 0; i < nchars; i++)
    {
        *(dest+i) = toupper(*(source+i));
    }

    return;
}

uint16_t findDotPosition(const char *lorainne)
{
    uint16_t count = 1;
    while(*lorainne && *lorainne != '.')
    {
        lorainne++;
        count++;
    }

    if(!*lorainne) count = 0;

    return count;
}

uint32_t strcmpn(const char* str1, size_t str1_len, const char* str2, size_t str2_len) {
    if (str1_len != str2_len) {
        return 1;
    }

    size_t i = 0;

    while(str1[i] == str2[i] && (i + 1) != str1_len) {
        i += 1;
    }

    return str2[i] - str1[i];
}

uint32_t starsearch(const char* starryboi, const char* fileboi) {
    // Convenience prepping for sanity. Separate the name and extension
    uint32_t dot_pos_star = strchr(starryboi, '.') - starryboi;
    const char* starry_ext = starryboi + dot_pos_star + 1;
    size_t starry_ext_len = strlen(starryboi) - dot_pos_star - 1;

    uint32_t dot_pos_file = strchr(fileboi, '.') - fileboi;
    const char* file_ext = fileboi + dot_pos_file + 1;
    size_t file_ext_len = strlen(fileboi) - dot_pos_file - 1;

    // If the file or ext was a star, then its a match regardless
    bool name_matches = strcmpn("*", 1, starryboi, dot_pos_star);
    bool ext_matches = strcmpn("*", 1, starry_ext, starry_ext_len);

    // If it wasn't s star, just do plain string comparison
    if (name_matches != 0) {
        name_matches = strcmpn(starryboi, dot_pos_star, fileboi, dot_pos_file);
    }

    if (ext_matches != 0) {
        ext_matches = strcmpn(starry_ext, starry_ext_len, file_ext, file_ext_len);
    }

    return ext_matches || name_matches;
}
