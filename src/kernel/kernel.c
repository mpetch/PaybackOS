#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

void main() {
    // Print out our welcome message
    printf("Thank you for using PaybackOS\n");
    if(debug()) {
        // Test liballoc
        void* testalloc = malloc(5);
        memcpy(testalloc, "hi!\n", 5);
        printf("liballoc test: %s", testalloc);
        free(testalloc);
    }
    // Halt the CPU with our shutdown interrupt
    asm("int $0");
    // In the case of cli and hlt failing we reboot
    asm("int $1");
}