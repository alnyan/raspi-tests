#include "sys/lib/string.h"

size_t strlen(const char *data) {
    size_t l;
    for (l = 0; *data; ++data, ++l);
    return l;
}
