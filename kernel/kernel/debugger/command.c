#include <stdio.h>
#include <string.h>
#include <stdint.h>

void dump_registers(void) {
    asm("mov $10, %eax; int $80");
}

// Define the handle_command function
void handle_command(char* command) {

    // You can add further functionality here, such as processing specific commands:
    if (strcmp(command, "REG") == 0) {
        dump_registers();
    } else if (strcmp(command, "SHUTDOWN") == 0) {
        clear_terminal();
        asm("mov $0, %eax; int $80");
    } else {
        kprintf("Unknown command: %s\n", command);
    }
    kprintf("> ");
}
