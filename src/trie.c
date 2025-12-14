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

const int CHILDREN_SIZE = 256;
// created a trie node 
typedef struct TrieNode {
    unsigned char char_byte;
    short int phrase_number; 
    Node *parent;
    Node *children[CHILDREN_SIZE];
} Node; 

// search result struct --> tells whether the byte stream exists along w the node 
typedef struct SearchResult {
    Node *searched_node;
    short int child_exists;
    unsigned char search_byte;
} Result;

// create the root node
Node* make_root() {
    Node *root_node=malloc(sizeof(Node));
    root_node->char_byte = "\0";
    root_node->phrase_number = 0;
    root_node->parent = NULL;

    for (int i = 0; i < CHILDREN_SIZE; i++)
    {
        root_node->children[i] = NULL;
    }

    return root_node;
}

// creates a node w/ character, parent node, and phrase number
Node* create_node(unsigned char character, Node* parent_node, short int phrase_number) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->char_byte = character;
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

    if (node->children[byte] == NULL) {
        Node* child_node = create_node(byte, node, phrase_number);
    }
}

// created search trie solution
Result* search_trie(unsigned char* byte_stream, Node* root) {
    Result search_result = {.searched_node = NULL, .child_exists = 1, .search_byte = NULL};

    Node* current_node = root;
    for (int i = 0; i < strlen(byte_stream); ++i) {
        if (current_node->children[byte_stream[i]] != NULL ) {
            current_node = current_node->children[byte_stream[i]];
        }
        else {
            search_result.searched_node = current_node;
            search_result.child_exists = 0; 
            search_result.search_byte = byte_stream[i];
            return &search_result;
        }
    }

    search_result.child_exists = 1; 

    return &search_result;
}

void image_compression(unsigned char* image_data, Node* root_node) {
    int phrase_number = 0;
    int start_index = 0;
    int end_index = 0;


    while (end_index < strlen(image_data)) {
        Result* search_result;
        search_result->search_byte=NULL;
        search_result->searched_node=NULL;
        search_result->child_exists=NULL;
        char* byte_stream = NULL;

        // copy string differently based on the size of the string
        // https://stackoverflow.com/questions/8600181/allocate-memory-and-save-string-in-c
        if (start_index == end_index){
            // allocate memory for the byte stream and copy character to bytestream
            byte_stream = (char* )malloc(sizeof(char));
            byte_stream = strcpy(byte_stream, byte_stream[start_index]);
            
            // search result from searching th etrie
            search_result = search_trie(byte_stream, root_node);
        }
        // https://cplusplus.com/reference/cstring/strncpy/
        // https://forums.raspberrypi.com/viewtopic.php?t=299281
        // https://stackoverflow.com/questions/6205195/given-a-starting-and-ending-indices-how-can-i-copy-part-of-a-string-in-c
        else if (end_index > start_index) {
            // determine length of substring and then allocate memory for that string
            int string_size = end_index-start_index;
            byte_stream = (char* )malloc((string_size+1)*sizeof(char));
            
            // copy new substring into bytestream
            strcnpy(byte_stream, image_data+start_index, string_size);
            byte_stream[string_size]="\0";

            // find the search result based on the bytestream
            search_result = search_trie(byte_stream, root_node);
        }

        // add a new node if a child is needed to add
        if (search_result->child_exists == 0) {
            insert_node(byte_stream, search_result->searched_node, phrase_number);
            
            ++phrase_number;
            start_index = end_index + 1;
            end_index = end_index + 1;
        }
        else {
            end_index = end_index + 1;
        }

        free(byte_stream);

    }
    /*
    general algorithm:
        - start at phrase number 0, index 0, end index 
        - while loop: (end index < strlen(image_data))
            - start at index 0
            - call search_trie  --> if result->child_exists = 0 --> insert node, increment phrase number using return nodebreak
                                --> if result->child_exists = 1 --> increase end index
    */ 
}


// https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
int create_trie() {
    // load the new image
    char* image_name = "rohan.jpg";
    int x_width;
    int y_height;
    int channel_num;
     
    // created image data using stb_image.h to find parameters for loading image
    unsigned char* image_data = stbi_load(image_name,&x_width, &y_height, &channel_num, 0);

    Node* root_node = make_root();
    image_compression(image_data, root_node);
    
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