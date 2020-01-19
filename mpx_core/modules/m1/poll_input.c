#include <core/serial.h>
#include <core/io.h>
#include <string.h>

int poll_input(char* buffer, int* length) {
    int max_length = *length;
    int current_i = 0;

    // Buffer to hold one char and a null byte for passing to serial_print
    char minibuf[2] = {0, 0};

    while (1) {
        if (inb(COM1 + 5) & 1) {
            // If input is available, read it
            char input = inb(COM1);

            if (0x20 <= input && input <= 0x7E) {
                // If the char is printable, add it to the buffer and print
                buffer[current_i] = input;
                current_i += 1;

                minibuf[0] = input;
                serial_print(minibuf);
            }

            else if (input == 0x7F && current_i > 0) {
                // If it's backspace then delete the last char in the buffer
                current_i -= 1;
                buffer[current_i] = 0;

                // \b moves the cursor one back. So this moves back, overwrites
                // with a space, and then moves back again.
                serial_print("\b \b");
            }

            if (input == 0x0A || input == 0x0D || current_i == max_length) {
                // 0A is newline, and 0D is carriage return. For some reason
                // when I hit enter I was receiving \r in the OS, so I just
                // check for both here for enter. If we hit enter, we print
                // a newline and return. We also return if we've hit the max
                // length
                serial_print("\n");
                *length = current_i;
                return 0;
            }
        }
    }
}
