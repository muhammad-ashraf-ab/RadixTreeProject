//
// Created by Ahmed Bakry on 1/11/2021.
//

#ifndef RADIXTREEPROJECT_RADIXTREE_H
#define RADIXTREEPROJECT_RADIXTREE_H

class RadixTree {
private:

    // Radix Tree's private inner class: Node
    class Node {
    public:

        // The value of the node
        char* key;

        // Number of characters in the node (includes the null character - if it exists)
        int len;

        Node* link;
        Node* next;

        // Basic constructor, initializes the node members as follows:
        // -- Node length:  n
        // -- Link node:    NULL
        // -- Next node:    NULL
        // -- Node value:   Loop sets character array
        Node(char* x, int n) : len(n), link(0), next(0)
        {
            // Create dynamic character array
            key = new char[n];

            // Set array
            for (int i = 0; i < n; i++) key[i] = x[i];
        }

        // Basic deconstructor, deletes node value
        ~Node() { delete[] key; }
    };

    // Radix Tree's root node
    Node* root;

    // --------------------------------------------------------------------------------
    // Prefix function, responsible for comparing two character arrays "x" and "key",
    // of lengths "n" and "m" respectively.
    int prefix(char* x, int n, char* key, int m);
    // Returns the number of common prefix characters
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Finder function, responsible for finding key "x" in tree of root node "t"
    Node* find(Node* t, char* x, int n = 0);
    // Returns the node corresponding to "x", if found
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Splitting function, 
    void split(Node* t, int k);
    // Returns
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Insertion function,
    Node* insert(Node* t, char* x, int n = 0);
    // Returns
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Joining function,
    void join(Node* t);
    // Returns
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Removal function,
    Node* remove(Node* t, char* x, int n = 0);
    // Returns
    // --------------------------------------------------------------------------------

public:

    // Basic constructor, initializes root node to NULL
    RadixTree() : root(0) {};


    void addString(char* str);
    void deleteString(char* str);
    bool searchString(char* str);

};

#endif //RADIXTREEPROJECT_RADIXTREE_H