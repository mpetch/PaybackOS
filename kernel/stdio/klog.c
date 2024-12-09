#include <stdio.h>
#include <stdbool.h>
#include <tty.h>

extern bool isdebug;

void klog(int level, const char* info) {
    if (level == 0 && isdebug) {
        terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
        kprint("Debug ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        kprintf("%s\n", info);
    } else if (level == 1) {
        terminal_setcolor(VGA_COLOR_GREEN);
        kprint("OK ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        kprintf("%s\n", info);
    } else if (level == 2) {
        terminal_setcolor(VGA_COLOR_LIGHT_RED);
        kprint("WARN ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        kprintf("%s\n", info);
    } else if (level == 3) {
        terminal_setcolor(VGA_COLOR_RED);
        kprint("CRIT ERR ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        kprintf("%s\n", info);
        asm("cli; hlt");
    }
}