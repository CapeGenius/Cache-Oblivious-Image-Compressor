# include <stdio.h> // i/o
# include <stdlib.h>


const int CHILDREN_SIZE = 256;
// created a trie node 
typedef struct TrieNode {
    unsigned char char_byte;
    short int phrase_number; 
    Node *parent;
    Node *children[CHILDREN_SIZE];
} Node; 

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

void insert_node(unsigned char char_byte, Node* node, int phrase_number) {
    int child_exists = 0; 
    
    if (node->children[char_byte] == NULL) {
        Node* child_node = create_node(char_byte, node, phrase_number);
    }
}