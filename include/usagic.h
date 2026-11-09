#ifndef USAGIC_H
#define USAGIC_H

#include <stddef.h>

#define USAGIC_VERSION "0.1.0-alpha"

/* Returns heap-allocated C source or NULL (error is written to err). */
char *usagic_transpile(const char *source, size_t size, const char *filename,
                       char *err, size_t err_cap);

#endif
