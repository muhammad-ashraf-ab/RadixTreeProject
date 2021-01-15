using namespace std;
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

        // The following two pointers are the core operational linker nodes of the radix tree

        // ---- The "next" node. This resembles a "sister" node to the current node, meaning that it is at the same tree level
        Node* next;

        // ---- The "link" node. This resembles a "child" node from the next tree level, not necessarily the only child
        Node* link;

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

    //File streams to print to right files
    ofstream generatedDNAFile;
    ofstream nodeCountFile;
    ofstream treeFile;

    // --------------------------------------------------------------------------------
    // Prefix function, responsible for comparing two character arrays "x" and "key",
    // of lengths "n" and "m" respectively.
    int prefix(char* x, int n, char* key, int m);
    // Returns the number of common prefix characters
    // --------------------------------------------------------------------------------
    void sortNodes(Node** arr, int size);
    void printStringsAUX(Node* t, char* prev, int prevlen);
    // --------------------------------------------------------------------------------
    // Finder function, responsible for finding key "x" in tree of root node "t"
    Node* find(Node* t, char* x, int n = 0);
    // Returns pointer to the node corresponding to "x", if found
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Splitting function, responsible for splitting a node into two at "k"
    // Used as part of the insertion process
    void split(Node* t, int k);
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Insertion function, responsible for inserting a node in its right position
    Node* insert(Node* t, char* x, int n = 0);
    // Returns pointer to the inserted node
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Joining function, responsible for joining a node with its link
    // Used as part of the removal process
    void join(Node* t);
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // Removal function, responsible for removing key "x" in tree of root node "t"
    Node* remove(Node* t, char* x, int n = 0);
    // Returns pointer to the node that takes place of removed node
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // counting strings function, responsible for counting strings in tree of root node "t"
    int countStringsAux(Node* t);
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    // counting nodes function, responsible for counting nodes in tree of root node "t"
    int countNodesAux(Node* t);
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    //prints a single node
    void printNodeAndPrefix(Node* n, char* prefix, int prefixlen);
    // --------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------
    void printNodesAUX(Node* current);

    // --------------------------------------------------------------------------------
    void printTreeAUX(char* prefix, const Node* node, int prefixLen = 0);

public:

    // Basic constructor, initializes root node to NULL
    RadixTree() : root(0) {};

    // public functions, names self-explanatory.
    void addString(char* str);
    void deleteString(char* str);
    bool searchString(char* str);
    int countStrings();
    int countNodes();
    void printNodes(const char* address);
    void printTree(const char* address);
    void printString(const char* address);
};

#endif //RADIXTREEPROJECT_RADIXTREE_H