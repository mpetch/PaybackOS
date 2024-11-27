#include <stdio.h>
#include <tty.h>
#include <serial.h>
#include <string.h>
#include <stdbool.h>
#include <boot/multiboot.h>

// Function prototypes
void idt_init(void);
void PIC_init();
void set_kernel_stack(uintptr_t);

// Stack for system calls
uint8_t esp0_stack[4096] __attribute__((aligned(4096)));

bool isdebug = false;

// Function pointer for the module's entry point
typedef void (*module_entry_t)(void);

void _init(multiboot_info_t* mb_info) {
    char* cmdline = (char*)(uintptr_t)mb_info->cmdline; // Cast to char pointer

    if (strstr(cmdline, "debug") != NULL) {
        isdebug = true;
    }

    // Initialize the terminal for output
    terminal_initialize();
    klog(0, "Started VGATM (VGA Text Mode)");

    // Initialize the serial port
    if (init_serial() == 0) {
        klog(0, "Serial port init successful");
    } else {
        klog(2, "Serial port init failed");
    }

    // Setup the kernel stack for interrupts
    set_kernel_stack((uintptr_t)(esp0_stack + sizeof(esp0_stack)));

    // Init the PIC (Programmable Interrupt Controller)
    PIC_init();
    klog(0, "PIC init successful");

    // Setup the IDT (Interrupt Descriptor Table)
    idt_init();
    klog(0, "IDT init successful");

    // Check for modules in the multiboot info
    if (mb_info->mods_count == 0) {
        klog(2, "No modules found!");
        return;
    }

    // Get the first module's start address
    multiboot_module_t* module = (multiboot_module_t*)(uintptr_t)mb_info->mods_addr;
    uintptr_t module_start = (uintptr_t)module->mod_start;

    // Cast the module start address to a function pointer
    module_entry_t entry = (module_entry_t)module_start;

    // Jump to the module's entry point
    klog(0, "Jumping to module...");
    entry();
}
