// THIS ALL NEEDS TO BE REFACTORED TO OUR PROJECT 
// STRUCTURE SCOPED IN OUR PROPOSAL FORM

// main.c
# include <stdio.h> // i/o
# include <stdlib.h> // standard library
# include <stdint.h> // exact width integer types
# include <string.h> // functions for manipulating strings
# include <time.h> // time functions to help derive metrics
// image processing library (single header library)
#define STB_IMAGE_IMPLEMENTATION 
# include "stb_image.h"
# include // normal LZ78 function
# include // cache oblivious LZ78 function

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
    return (double)(end - start) / (double)CLOCKS_PER_SEC
}

// Load image and treat its raw pixels as a flat byte array
static uint8_t *load_image_as_bytes(const char *filename, size_t *out_size) {
    int width;
    int height;
    int channels; // # of color components per pixed (3 --> rgb)
    // data is going to store our pixel data in a processable form
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0) {
        if (!data) {
            fprintf(stderr, "Error: could not load image '%s'\n", filename);
            return NULL;
        }
    // Compute the total size of our image stored in data
    size_t size = (size_t)width * (size_t)height * size_t(channels);
    // Write 'size' into the size_t variable that out_size points to (dereferencing)
    *out_size = size;
    // return data, but typecast it to uin8_t
    return (uint*_t)data;
    }
}

// Stores the data bytearray in disk
static int write_binary_file(const char *filename, const uint8_t *data, size_t size) {
    // opens a file called filename in wb mode, gives pointer (f) that will write bytes into that file.
    FILE *f = fopen(filename, "wb");
    // if f is null, an issue occured
    if (!f) {
        fprintf(stderr, "Error: could not open '%s' for writing\n", filename);
        return 0;
    }
    // Check if we wrote the correct amount of bytes into *f
    if (fwrite(data, 1, size, f) != size) {
        fprintf(stderr, "Error: failed to write all bytes to '%s'\n", filename);
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

// Print statistics function
static void print_stats(const char *label, const CompressionStats *s) {
    // Get the compression ratio 
    double ratio = (s -> compressed_size > 0) ? (double)s->originial_size / (double)s->compressed_size : 0.0
    printf("=== %s ===\n", label);
    printf("Original size     : %zu bytes\n", s->original_size);
    printf("Compressed size   : %zu bytes\n", s->compressed_size);
    printf("Compression ratio : %.3fx\n", ratio);
    printf("Time              : %.6f s\n", s->seconds);
    printf("---------------------------\n");
}

// Main function
int main(int argc, char **argv) {
    // If we didn't put the correct number of CL arguments
    if (arc != 4) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    const char *mode = argv[1]; // naive, cache-oblivious, both
    const char *input_image = argv[2];
    const char *output_base = argv[3];
    // Use our load_image_as_bytes function to create a byte array representing the image
    size_t original_size = 0;
    uint8_t *image_bytes = load_image_as_bytes(input_image, &original_size);
    if (!image_bytes) {
        return EXIT_FAILURE;
    }
    // If everything was successful, we we loaded our image into RAM
    printf("Loaded '%s' (%zu bytes of pixel data)\n"), input_image, original_size);
    // Check if we need to run our naive LZ
    int run_naive = (strcmp(mode, "naive") == 0) || (strcmp(mode, "both") == 0);
    // Check if we need to run our cache-oblivious LZ
    int run_co = (strcmp(mode, "co") == 0) || (strcmp(mode, "both") == 0);
    // if we aren't running either of them, then something is wrong
    if (!run_naive && !run_co) {
        fprintf(stderr, "Error: mode must be 'naive', 'co', or 'both\n");
        // let the heap allocator know to free image_bytes
        stbi_image_free(image_bytes);
        return EXIT_FAILURE;
    }
    // Naive LZ78
    if (run_naive) {
        uint8_t *compressed = NULL; // will end up storing compressed byte array
        clock_t start = clock();
        size_t compressed_size = lz78_naive_compress(image_bytes, original_size, &compressed) // need to come back to this
        clock_t end = clock();
        // Check if compressed size and compressed are values that make sense
        if (compressed_size == 0 || !compressed) {
            fprintf(stderr, "Error: naive compression failed\n");
        } else {
            CompressionStats stats = {
                .seconds = elapsed_seconds(start, end),
                .original_size = original_size,
                .compressed_size = compressed_size
            };
        }
        // get the output file name, and store it in out_name
        char out_name[512];
        sprintf(out_name, sizeof(out_name), "%s.naive.lz78", output_base);
        // Use our write_binary_function to write our bytes to a file
        if (!write_binary_file(out_name, compressed, compressed_size)) {
            fprint(stderr, "Warning: could not write naive output file '%s'\n", out_name);
        } else {
            printf("Naive compressed output written to '%s'\n", out_name);
        }
        print_stats("Naive LZ78", &stats);
    }
    // The LZ78 compressor mallocs this buffer and returns ownership to us.
    // After writing the output file, we must free it to avoid a memory leak.
    free(compressed);
    // Cache-oblivious LZ78
    if (run_co) {
        uint8_t *compressed = NULL;
        clock_t start = clock();
        size_t compressed_size = lz78_cache_oblivious_compress(image_bytes, original_size, &compressed);
        clock_t end = clock();
        if (compressed_size == 0 || !compressed) {
            fprintf(stderr, "Error: cache-oblivious compression failed\n");
        } else {
            CompressionStats stats = {
                .seconds = elapsed_seconds(start, end),
                .original_size = original_size,
                .compressed_size = compressed_size
            };
            char out_name[512];
            snprintf(out_name, sizeof(out_name), "%s.co.lz78", output_base);
            if (!write_binary_file(out_name, compressed, compressed_size)) {
                fprintf(stderr,
                        "Warning: could not write cache-oblivious output '%s'\n", out_name);
            } else {
                printf("Cache-oblivious compressed output written to '%s'\n", out_name);
            }
            print_stats("Cache-Oblivious LZ78", &stats);
        }
        free(compressed);
    }
    stbi_image_free(image_bytes);
    return EXIT_SUCCESS;
}


