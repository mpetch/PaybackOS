#ifndef STDIO_H
#define STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void print(const char* str);
void putchar(char c);
void klog(int level, const char* msg);
void printf(const char* format, ...);
char getch();
void halt();
bool checkdebug();
bool fulldebug();

#ifdef __cplusplus
}
#endif

#endif // STDIO_H
