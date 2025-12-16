// main.c
# include <stdio.h> // i/o
# include <stdlib.h> // standard library
# include <stdint.h> // exact width integer types
# include <string.h> // functions for manipulating strings
# include <time.h> // time functions to help derive metrics
// image processing library (single header library)

// include your helper headers
#include "cache_naive_helpers.h"
#include "trie.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple struct to hold timirg + size metrics
// Creates the CompressionStats type
typedef struct {
    double seconds;
    size_t original_size;
    size_t compressed_size;
} CompressionStats;

// Static function which prints an error message if user incorrectly 
// entered in program run command at CLI
static void print_usage(const char *progname) {
    fprintf(stderr,
        "Usage: %s <mode> <input_image> <output_basename>\n"
        "  mode: naive | co | both\n"
        "  example: %s both input.png out\n",
        progname, progname
    );
}

// Static function to find elapsed time
static double elapsed_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

// -----------------------------------------------
// Load image → flat byte array
// -----------------------------------------------
static uint8_t *load_image_as_bytes(const char *filename, size_t *out_size) {
    int width, height, channels;
    puts("loading image");
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    puts("image loaded");

    if (!data) {
        fprintf(stderr, "Error: could not load image '%s'\n", filename);
        return NULL;
    }

    // printf("the width is %d", width);

    int size = width * height * channels;
    // printf("size is %s", data);
    *out_size = size;
    return (uint8_t *)data;
}

// -----------------------------------------------
// Write binary file
// -----------------------------------------------
static int write_binary_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Error: could not open '%s' for writing\n", filename);
        return 0;
    }
    if (fwrite(data, 1, size, f) != size) {
        fprintf(stderr, "Error: failed to write all bytes to '%s'\n", filename);
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

// -----------------------------------------------
// Print stats
// -----------------------------------------------
static void print_stats(const char *label, const CompressionStats *s) {
    double ratio = (s->compressed_size > 0)
        ? (double)s->original_size / (double)s->compressed_size
        : 0.0;

    printf("=== %s ===\n", label);
    printf("Original size     : %zu bytes\n", s->original_size);
    printf("Compressed size   : %zu bytes\n", s->compressed_size);
    printf("Compression ratio : %.3fx\n", ratio);
    printf("Time              : %.6f s\n", s->seconds);
    printf("---------------------------\n");
}

// -----------------------------------------------
// MAIN
// -----------------------------------------------
int main(int argc, char** argv) {

    if (argc != 2) {
        return EXIT_FAILURE;
    }

    char* compression_type = argv[1];
    
    if (strcmp(compression_type, "trie") == 0) {
        create_trie();
    }

    if (strcmp(compression_type, "dict") == 0) {

        // https://www.ibm.com/docs/en/i/7.5.0?topic=functions-main-function
        size_t original_size;
        const char* filename = "/home/rohanbendapudi/Cache-Oblivious-Image-Compressor/src/static/rohan.jpg";
        uint8_t *image_bytes = load_image_as_bytes(filename, &original_size);

        if (image_bytes == NULL) {
            puts("image is null \n");
        }

        uint8_t *compressed = NULL;
        printf("original size %zu", original_size);

        // clock_t start = clock();
        size_t compressed_size = lz78_naive_compress(image_bytes, original_size, &compressed);
        // clock_t end = clock();
        free(compressed);
    }
}
