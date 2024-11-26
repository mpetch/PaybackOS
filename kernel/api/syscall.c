#include <stdint.h>
#include <tty.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern bool isdebug;

// Define our registers that we can use in our syscall handler
typedef struct
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, useless_esp, ebx, edx, ecx, eax;
	uint32_t int_num, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) stack_frame_t;

// Define syscalls
#define Halt 0 // Halt the CPU
#define Print 1 // Print a string to the string
#define Putchar 2 // Print a char to the screen
#define Log 3 // Log with levels
#define Checkdebug 5 // Check if the OS is in debug mode
#define Malloc 6 // Malloc
#define Free 7 // Free
#define Calloc 8 // Calloc
#define Realloc 9 // Realloc
#define Stack_Frame 10 // return the current stack frame

// Our system call function, this is called by int $80
void syscall_handler(stack_frame_t *frame) {
    if (frame->eax == Halt) {
        klog(2, "Halting CPU.\n");
        asm("cli; hlt");
    } else if (frame->eax == Print) {
        kprintf("%s", (char*)frame->ebx);
        return;
    } else if (frame->eax == Putchar) {
        kputchar(frame->ebx);
        return;
    } else if (frame->eax == Log) {
        klog(frame->ebx, (const char*)frame->ecx); // Logging
        return;
    } else if (frame->eax == Checkdebug) {
        frame->eax = isdebug;
        return;
    } else if (frame->eax == Malloc) {
        void* ptr = kmalloc(frame->ebx);
        frame->eax = (uint32_t)ptr; // We onlt make it a uint32_t here since the types are not compatible but that is the return register
        return;
    } else if (frame->eax == Free) {
        kfree((void*)frame->ebx); // Assume ebx is the pointer
    } else if (frame->eax == Calloc) {
        /*the EAX register is used as return*/frame->eax = (uint32_t)(frame->ebx, frame->ecx); // Calloc the memory
        return;
    } else if (frame->eax == Realloc) {
        /*the EAX register is used as return*/frame->eax = (uint32_t)krealloc((void*)frame->ebx, frame->ecx); // Resize the pointer
        return;
    } else if (frame->eax == Stack_Frame) {
        // Print the entire stack frame
        kprintf("gs: %x\n", frame->gs);
        kprintf("fs: %x\n", frame->fs);
        kprintf("es: %x\n", frame->es);
        kprintf("ds: %x\n", frame->ds);
        kprintf("edi: %x\n", frame->edi);
        kprintf("esi: %x\n", frame->esi);
        kprintf("ebp: %x\n", frame->ebp);
        kprintf("ebx: %x\n", frame->ebx);
        kprintf("edx: %x\n", frame->edx);
        kprintf("ecx: %x\n", frame->ecx);
        kprintf("eax: %x\n", frame->eax);
        kprintf("int num: %x\n", frame->int_num);
        kprintf("eip: %x\n", frame->eip);
        kprintf("cs: %x\n", frame->cs);
        kprintf("eflags: %x\n", frame->eflags);
        kprintf("useresp: %x\n", frame->useresp);
        kprintf("ss: %x\n", frame->ss);
        return;
    } else {
        kprintf("Interrupt %x is not a real syscall", frame->int_num);
    }
    return; // If no valid syscall number, do nothing
}