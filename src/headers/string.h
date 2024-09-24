#ifndef STRING_H
#define STRING_H 1
#pragma once

#include <stddef.h>

void* memcpy(void* restrict dest, const void* restrict src, size_t size);
size_t strlen(const char* str);
void* memmove(void* dest, const void* src, size_t size);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
int strcmp(const char *str1, const char *str2);
char* strcpy(char* dest, const char* src);

#endif