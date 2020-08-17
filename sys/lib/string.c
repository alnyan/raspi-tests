#include "sys/lib/string.h"

size_t strlen(const char *data) {
    size_t l;
    for (l = 0; *data; ++data, ++l);
    return l;
}

void *memset(void *block, int value, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        ((unsigned char *) block)[i] = value;
    }
    return block;
}
