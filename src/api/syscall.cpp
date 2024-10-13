#include <stdint.h>
#include <tty.hpp>
#include <stdio.hpp>

// Define our function prototypes
void ide_read(uint32_t lba, uint16_t sectors, void *buffer);

// Define our registers that we can use in our syscall handler
typedef struct
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, useless_esp, ebx, edx, ecx, eax;
	uint32_t int_num, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) stack_frame_t;

// Define syscalls
#define SYSCALL_HALT 0 // Halt the CPU
#define SYSCALL_PRINT 1 // Print a string to the string
#define SYSCALL_PUTCHAR 2 // Print a char to the screen
#define SYSCALL_LOG 3 // Log with levels
#define SYSCALL_READ 4 // Read from disk

// Our system call function, this is called by int $80
void syscall_handler(stack_frame_t *frame) {
    if (frame->eax == SYSCALL_HALT) {
        klog(3, "Please reboot PC, Major error occured.\n");
        asm("cli; hlt");
    } else if (frame->eax == SYSCALL_PRINT) {
        kprintf("%s", (char*)frame->ebx); // Assuming ebx points to a string
        return;
    } else if (frame->eax == SYSCALL_PUTCHAR) {
        vga::putchar(frame->ebx); // Assuming ebx is a character
        return;
    } else if (frame->eax == SYSCALL_LOG) {
        klog(frame->ebx, (const char*)frame->ecx); // Logging
        return;
    } else if (frame->eax == SYSCALL_READ) {
        void* buffer[512]; // Setup our buffer for the disk
        ide_read(frame->ebx, frame->ecx, buffer); // Read from the disk and put it into here
        frame->eax = (unsigned int)buffer; // Put data into the return register
        return;
    }
    return; // If no valid syscall number, do nothing
}
