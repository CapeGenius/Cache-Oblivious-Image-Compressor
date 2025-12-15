# include <stdio.h> // i/o
# include <stdlib.h>
# include <string.h>
# include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// C Standard Library - https://www.programiz.com/c-programming/library-function/string.h/strlen
// geeks for geeks: https://www.geeksforgeeks.org/c/implementation-of-trie-prefix-tree-in-c/
// Compact Data Structures by Navarro
// https://www.reddit.com/r/C_Programming/comments/sx1xpl/since_the_size_of_datatypes_are_not_fixed_in_c/
// https://pubs.opengroup.org/onlinepubs/007904975/basedefs/stdint.h.html --> need to look into for uint_t
// https://github.com/nothings/stb/blob/master/stb_image.h

#define CHILDREN_SIZE 256
// created a trie node 
struct TrieNode {
    u_int8_t byte;
    short int phrase_number; 
    struct TrieNode *parent;
    struct TrieNode *children[CHILDREN_SIZE];
}; 

typedef struct TrieNode Node;

// search result struct --> tells whether the byte stream exists along w the node 
typedef struct SearchResult {
    Node *searched_node;
    short int child_exists;
    unsigned char search_byte;
} Result;

// creates a node w/ character, parent node, and phrase number
Node* create_node(unsigned char character, Node* parent_node, short int phrase_number) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->byte = character;
    node->parent = parent_node;
    node->phrase_number = phrase_number;

    for (int i = 0; i < CHILDREN_SIZE; i++)
    {
        node->children[i] = NULL;
    }

    return node;
}

// initially thought of recursively inserting nodes --> search for nodes and add nodes for LZ78
void insert_node(unsigned char byte, Node* node, int phrase_number) {
    // printf("pointer to byte is %p \n", node->children[byte]);
    node->children[(int)byte] = create_node(byte, node, phrase_number);
}

// created search trie solution
Result* search_trie(unsigned char* byte_stream, Node* root) {
    Result* search_result = malloc(sizeof(Result));
    search_result->search_byte=0;
    search_result->child_exists=1;


    Node* current_node = root; 
    for (int i = 0; i < strlen((const char* )byte_stream); ++i) {
        // printf("current byte is: %u \n",byte_stream[i]);
        // printf("the child: %p \n", current_node->children[byte_stream[i]]);
        if (current_node->children[byte_stream[i]] != NULL ) {
            current_node = current_node->children[byte_stream[i]];
        }
        else {
            search_result->searched_node = current_node;
            search_result->child_exists = 0; 
            search_result->search_byte = byte_stream[i];
            // puts("found a node");
            return search_result;
        }
    }

    search_result->child_exists = 1; 

    return search_result;
}

void image_compression(unsigned char* image_data, Node* root_node, size_t image_size) {
    int phrase_number = 0;
    int start_index = 0;
    int end_index = 0;

    puts("welcome to image compression \n");

    while (end_index < image_size) {
        // printf("phrase number: %d \n", phrase_number);
        // printf("end index: %d \n, image size:%zu \n", end_index, strlen(image_data));
        Result* search_result = (Result*)malloc(sizeof(Result));
        search_result->search_byte=0;
        search_result->searched_node=(Node* ) malloc(sizeof(Node));
        search_result->child_exists=1;

        // copy string differently based on the size of the string
        // https://stackoverflow.com/questions/8600181/allocate-memory-and-save-string-in-c
        if (start_index == end_index){

            unsigned char * byte_stream = (unsigned char*) malloc(2*sizeof(unsigned char));
            // puts("memory allocated");
            strncpy(byte_stream, image_data+start_index, 1);
            byte_stream[1] = '\0';
            // printf("byte stream is %s \n", byte_stream);
            // search result from searching the trie
            // printf("the pointer is %p \n", (void *)root_node);
            search_result = search_trie(byte_stream, root_node);
            

            // puts("what's up");

            // add a new node if a child is needed to add
            if (search_result->child_exists == 0) {
                insert_node(search_result->search_byte, search_result->searched_node, phrase_number);
                
                ++phrase_number;
                start_index = end_index + 1;
                end_index = end_index + 1;
            }
            else {
                // puts("went here instead \n");
                end_index = end_index + 1;
            }
            free(byte_stream);
        }


        // https://cplusplus.com/reference/cstring/strncpy/
        // https://forums.raspberrypi.com/viewtopic.php?t=299281
        // https://stackoverflow.com/questions/6205195/given-a-starting-and-ending-indices-how-can-i-copy-part-of-a-string-in-c
        else if (end_index > start_index) {
            // determine length of substring and then allocate memory for that string
            int string_size = end_index-start_index;
            unsigned char* byte_stream = (unsigned char* )malloc((string_size+1)*sizeof(unsigned char));
            // copy new substring into bytestream
            strncpy(byte_stream, image_data+start_index, string_size);
            byte_stream[string_size]="\0";

            // find the search result based on the bytestream
            search_result = search_trie(byte_stream, root_node);

            // add a new node if a child is needed to add
            if (search_result->child_exists == 0) {
                insert_node(search_result->search_byte, search_result->searched_node, phrase_number);
                printf("the string size %d \n", string_size);
                
                ++phrase_number;
                start_index = end_index + 1;
                end_index = end_index + 1;
            }
            else {

                end_index = end_index + 1;
            }

            free(byte_stream);
        }
        // puts("---------------------------------------------------------------");
        free(search_result);

    }
    printf("end_index is %d", end_index);
    printf("the phrase number was %d \n", phrase_number);
    /*
    general algorithm:
        - start at phrase number 0, index 0, end index 
        - while loop: (end index < strlen(image_data))
            - start at index 0
            - call search_trie  --> if result->child_exists = 0 --> insert node, increment phrase number using return nodebreak
                                --> if result->child_exists = 1 --> increase end index
    */ 
}


// https://stackoverflow.com/questions/14067403/valgrind-invalid-read-of-size-1
// https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
int create_trie() {
    // load the new image
    char* image_name = "/home/rohanbendapudi/Cache-Oblivious-Image-Compressor/src/static/cat_image.jpg";
    int x_width;
    int y_height;
    int channel_num;
    Node* root_node_ptr = create_node((unsigned char )0, NULL, 0);
     
    // created image data using stb_image.h to find parameters for loading image
    char* image_data = (char*) stbi_load(image_name,&x_width, &y_height, &channel_num, 0);
    // unsigned char* image_data="hellllo";
    // printf("image is %s \n", image_data);

    printf("size of image %zu \n", (size_t)(sizeof(image_data)/sizeof(u_int8_t)));
    size_t image_size = (size_t) x_width*y_height*channel_num;
    printf("The size of the image is %d", image_size);

    if (image_data != NULL) {
        puts("Hello");
        
        // printf("size of image %zu", strlen((char*)image_data));
        image_compression(image_data, root_node_ptr, image_size);
    }
    else {
        puts("Hello");
    }
    
    printf("size of node: %zu", sizeof(Node));
    // create image --> used stb_image.h documentation to find parameters for loading image
    return 0;
}

int main() {
    create_trie();
}

// used this to use stbi_load STBIDEF stbi_uc *stbi_load            
//(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
//// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data);