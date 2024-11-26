#include <stddef.h>

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;  // Cast pointer to unsigned char*
    while (num--) {
        *p = (unsigned char)value;  // Set each byte to the specified value
        p++;  // Move to the next byte
    }
    return ptr;  // Return the original pointer
}
