// main.c
# include <stdio.h> // i/o
# include <stdlib.h> // standard library
# include <stdint.h> // exact width integer types
# include <string.h> // functions for manipulating strings
# include <time.h> // time functions to help derive metrics
// image processing library (single header library)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// include your helper headers
#include "cache_naive_helpers.h"

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
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        fprintf(stderr, "Error: could not load image '%s'\n", filename);
        return NULL;
    }

    size_t size = (size_t)width * (size_t)height * (size_t)channels;
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
int main(int argc, char **argv) {

    if (argc != 4) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *mode        = argv[1];
    const char *input_image = argv[2];
    const char *output_base = argv[3];

    // Load image
    size_t original_size = 0;
    uint8_t *image_bytes = load_image_as_bytes(input_image, &original_size);
    if (!image_bytes) return EXIT_FAILURE;

    printf("Loaded '%s' (%zu bytes of pixel data)\n", input_image, original_size);

    int run_naive = (strcmp(mode, "naive") == 0 || strcmp(mode, "both") == 0);
    int run_co    = (strcmp(mode, "co")    == 0 || strcmp(mode, "both") == 0);

    if (!run_naive && !run_co) {
        fprintf(stderr, "Error: mode must be 'naive', 'co', or 'both'\n");
        stbi_image_free(image_bytes);
        return EXIT_FAILURE;
    }
    // Naive LZ78
    if (run_naive) {
        uint8_t *compressed = NULL;

        clock_t start = clock();
        size_t compressed_size =
            lz78_naive_compress(image_bytes, original_size, &compressed);
        clock_t end = clock();

        if (compressed_size == 0 || !compressed) {
            fprintf(stderr, "Error: naive compression failed\n");
        } else {
            CompressionStats stats = {
                .seconds         = elapsed_seconds(start, end),
                .original_size   = original_size,
                .compressed_size = compressed_size
            };

            char out_name[512];
            snprintf(out_name, sizeof(out_name), "%s.naive.lz78", output_base);

            if (!write_binary_file(out_name, compressed, compressed_size)) {
                fprintf(stderr, "Warning: failed to write '%s'\n", out_name);
            } else {
                printf("Naive compressed output written to '%s'\n", out_name);
            }

            print_stats("Naive LZ78", &stats);
        }
        free(compressed);
    }

    // ---------------------------
    // CACHE-OBLIVIOUS (later)
    // ---------------------------
    // if (run_co) {
    //     uint8_t *compressed = NULL;

    //     clock_t start = clock();
    //     size_t compressed_size =
    //         lz78_cache_oblivious_compress(image_bytes, original_size, &compressed);
    //     clock_t end = clock();

    //     if (compressed_size == 0 || !compressed) {
    //         fprintf(stderr, "Error: cache-oblivious compression failed\n");
    //     } else {
    //         CompressionStats stats = {
    //             .seconds         = elapsed_seconds(start, end),
    //             .original_size   = original_size,
    //             .compressed_size = compressed_size
    //         };

    //         char out_name[512];
    //         snprintf(out_name, sizeof(out_name), "%s.co.lz78", output_base);

    //         if (!write_binary_file(out_name, compressed, compressed_size)) {
    //             fprintf(stderr,
    //                     "Warning: could not write cache-oblivious output '%s'\n", out_name);
    //         } else {
    //             printf("Cache-oblivious compressed output written to '%s'\n", out_name);
    //         }

    //         print_stats("Cache-Oblivious LZ78", &stats);
    //     }
    //     free(compressed);
    // }

    stbi_image_free(image_bytes);
    return EXIT_SUCCESS;
}
