# include <stdio.h> // i/o
# include <stdlib.h>
#include <string.h>

// C Standard Library - https://www.programiz.com/c-programming/library-function/string.h/strlen
// geeks for geeks: https://www.geeksforgeeks.org/c/implementation-of-trie-prefix-tree-in-c/
// Compact Data Structures by Navarro

const int CHILDREN_SIZE = 256;
// created a trie node 
typedef struct TrieNode {
    char char_byte;
    short int phrase_number; 
    Node *parent;
    Node *children[CHILDREN_SIZE];
} Node; 

// search result struct --> tells whether the byte stream exists along w the node 
typedef struct SearchResult {
    Node *searched_node;
    short int child_exists;
    char search_byte;
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
Node* create_node(const char character, Node* parent_node, short int phrase_number) {
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
void insert_node(const char byte, Node* node, int phrase_number) {

    if (node->children[byte] == NULL) {
        Node* child_node = create_node(byte, node, phrase_number);
    }
}

// created search trie solution
Result* search_trie(const char* byte_stream, Node* root) {
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