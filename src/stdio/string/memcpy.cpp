#include <stddef.h>

void memcpy(void *dest, void *src, size_t n) { 
	// Typecast src and dest addresses to (char *) 
	char *csrc = (char *)src; 
	char *cdest = (char *)dest; 

	// Copy contents of src[] to dest[] 
	for (size_t i=0; i<n; i++) 
	    cdest[i] = csrc[i]; 
} 
