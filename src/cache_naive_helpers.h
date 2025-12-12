#ifndef CACHE_NAIVE_HELPERS_H
#define CACHE_NAIVE_HELPERS_H

#include <stddef.h>
#include <stdint.h>

size_t lz78_naive_compress(
    const uint8_t *input,
    size_t input_size,
    uint8_t **output);

#endif
