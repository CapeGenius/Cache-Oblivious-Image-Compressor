# include <stdio.h> // i/o
# include <stdlib.h>

// created a trie node 
typedef struct TrieNode {
    unsigned char* byte;
    struct TrieNode *children[256];
} Node; 