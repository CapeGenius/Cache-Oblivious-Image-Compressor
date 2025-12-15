#pragma once

#define CHILDREN_SIZE 256

struct TrieNode {
    unsigned char byte;
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


Node* create_node(unsigned char character, Node* parent_node, short int phrase_number);

void insert_node(unsigned char byte, Node* node, int phrase_number);

void search_trie(unsigned char* byte_stream, Node* root, Result* search_result, int string_size);