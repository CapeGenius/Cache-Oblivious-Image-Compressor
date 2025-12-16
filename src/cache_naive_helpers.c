#include "cache_naive_helpers.h"
#include <stdint.h>
#include <stdlib.h>

#define DICT_CAPACITY 1024

// Dictionary entry
typedef struct {
    int prefix; // which previous dictionary entry to start from
    uint8_t symbol; // You take an existing dictionary phrase, and you add one more byte to the end of it to make a longer phrase.
} DictEntry;

size_t lz78_naive_compress(const uint8_t *input, size_t input_size, uint8_t **output)
{
    // input is a pointer to the original data, and length of the input data
    // allocate dictionary
    size_t dict_cap = DICT_CAPACITY; // max entries for dictionaries
    size_t dict_size = 0; // current number of entries
    DictEntry *dict = malloc(dict_cap * sizeof(DictEntry)); // allocates block of memory to hold 1024 DictEntry structures 

    // allocate output buffer
    size_t out_cap = input_size * 2; // max data for current buffer, *2 is a guess
    size_t out_size = 0; // current size of compressed data
    uint8_t *out = malloc(out_cap);

    size_t i = 0;

    while (i < input_size)
    {
        int prefix = 0;
        size_t j = i;

        while (j < input_size) {
            uint8_t c = input[j];

            // linear dictionary search (cache naive)
            int found = 0;
            for (size_t d = 1; d <= dict_size; d++) {
                if (dict[d - 1].prefix == prefix &&
                    dict[d - 1].symbol == c)
                {
                    // match found!
                    prefix = (int)d;
                    found = 1; 
                    break;
                }
            }

            if (!found) // combination of the longest match plus the new character c does not exist in dictionary
            {
                // output: prefix (2 bytes) + symbol(1 byte)
                if (out_size + 3 > out_cap) {
                    out_cap *= 2;
                    out = realloc(out, out_cap);
                }

                out[out_size++] = (prefix >> 8) & 0xFF;
                out[out_size++] = (prefix & 0xFF);
                out[out_size++] = c;

                // extend dictionary
                if (dict_size == dict_cap) {
                    dict_cap *= 2;
                    dict = realloc(dict, dict_cap * sizeof(DictEntry));
                }
                //dictionary grows one entry
                dict[dict_size++] = (DictEntry){ .prefix = prefix, .symbol = c };

                j++;
                break;
            }
            j++;
        }

        i = j;
    }

    free(dict);

    *output = out;
    return out_size;
}
