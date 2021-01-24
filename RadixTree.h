//---------------------------------------------------------------------------------------------------------------------------------------------
// This project was created for CSE_331 Data Structures And Algorithms course offered in
// Ain Shams University - Faculty of Engineering under the guidance and influence of Dr. Ashraf Abdel Raouf
//
// This implementation has been greatly influenced by the implementation found in the following source:
// https://kukuruku.co/post/radix-trees/
//---------------------------------------------------------------------------------------------------------------------------------------------
#ifndef RADIXTREEPROJECT_RADIXTREE_H
#define RADIXTREEPROJECT_RADIXTREE_H
#include <fstream>
using namespace std;

class RadixTree {
private:

    // Radix Tree's private inner class: Node
    class Node {
    public:

        // The following two node pointers represent the core operational linker nodes of the Radix Tree

        // ---- The "link" node. This resembles the first "child" node to the current node from the next tree level
        Node* link;

        // ---- The "next" node. This resembles a "sibling" node to the current node, meaning that it is at the same tree level
        Node* next;

        // The value, or "key", of the node, which is not unique per node, and may or may not contain a null terminator
        char* key;

        // Number of characters in the node (includes the null character - if it exists)
        int len;

        // Basic constructor, initializes the node members as follows:
        // -- Node length:  n
        // -- Link node:    NULL
        // -- Next node:    NULL
        // -- Node value:   Loop sets character array
        //
        Node(const char* x, int n) : link(0), next(0), len(n) {

            key = new char[len];
            for (int i = 0; i < len; i++) key[i] = x[i];

        }

        // Shallow Copy constructor, only sets the pointers regarding "link" and "next" in addition to key
        Node(const Node* orig) : link(orig->link), next(orig->next), len(orig->len) {

            key = new char[len];
            for (int i = 0; i < len; i++) key[i] = orig->key[i];

        }

        // Deep Copy constructor, clones the original node (i.e. makes this node an exact copy of node "orig")
        Node(const Node& orig) : len(orig.len) {

            key = new char[len];
            for (int i = 0; i < len; i++) key[i] = orig.key[i];
            link = orig.link ? new Node(*orig.link) : 0;
            next = orig.next ? new Node(*orig.next) : 0;

        }

        // Equality operator overloading
        bool operator==(const Node& rhs) {

            if (len != rhs.len) return false; // Compare lengths first, if they do not match then they can't be equal therefore return false
            for (int i = 0; i < len; i++) if (key[i] != rhs.key[i]) return false; // Next, compare each character, on conflict return false
            if (*link != *(rhs.link) || *next != *(rhs.next)) return false; // Finally, compare if its first child or sibling are the same
            // Notice that the check above recursively checks all of the children and siblings of these two nodes - aka sub-tree checking
            return true; // If comparison reaches this line then the two nodes are indeed congruent to each other, therefore return true

        }

        // Inequality operator overloading - just like the naming, it is literally the opposite of the equality operator
        bool operator!=(const Node& rhs) { return !(*this == rhs); }

        // Basic deconstructor, de-allocates memory occupied by node's key, children, and siblings (i.e. its sub-tree)
        // The sub-tree deletion works via recursive deletion of children and siblings, since calling "delete link"...
        // ...calls this function again (i.e. undergoes recursion) and the same with "delete next" where ultimately...
        // ...all of its children and siblings will have been deleted.
        //
        ~Node() { delete link; delete next; delete[] key; }

    };

    // Radix Tree's root node
    Node* root;

    // File streams to print different outputs to their respective files
    ofstream segmentsFile;
    ofstream nodesFile;
    ofstream treeFile;

    // ---------------------------------------------------------------------------------------------------------------
    // Prefix function, responsible for comparing two character arrays "x" and "key"...
    // ...of lengths "n" and "m" respectively.
    //
    int prefix(char* x, int n, char* key, int m);
    // Returns the number of common prefix characters
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Finder (Search) function, responsible for finding key "x" in tree of root node "t"
    //
    Node* find(Node* t, char* x, int n = 0);
    // Returns pointer to the node corresponding to "x", if found
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Splitting function, responsible for splitting a node into two at position "k"
    // Used as part of the insertion process
    //
    void split(Node* t, int k);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Insertion function, responsible for inserting a node in its right position
    //
    Node* insert(Node* t, char* x, int n = 0);
    // Returns pointer to the inserted node
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Joining function, responsible for joining a node with its link
    // Used as part of the removal process
    //
    void join(Node* t);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Removal function, responsible for removing key "x" in tree of root node "t"
    //
    Node* remove(Node* t, char* x, int n = 0);
    // Returns pointer to the node that takes place of removed node
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary string counting function, responsible for counting strings in tree of root node "t"
    //
    int countStringsAux(Node* t);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary node counting function, responsible for counting nodes in tree of root node "t"
    //
    int countNodesAux(Node* t);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary tree sorting function, responsible for sorting the Radix Tree by sorting its different sub-trees...
    // ...of node linked lists, given the head (initialized as root) of the (sub-)tree(s), i.e. sibling linked lists
    //
    // It is to be noted that using the function without setting the new root as the return value of the function...
    // ...leaves us with an outdated root node pointer that may point towards a sub-tree rather than the sorted tree
    //
    // It is used as part of the string fetching function which provides it a clone tree to sort so as to fetch from
    //
    // On its own, it will recursively sort the Radix Tree whose root node "head" is provided to the function
    //
    Node* sortRadixTreeAux(Node* head);
    // Returns pointer to the root node of the sorted Radix Tree
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary string fetching function, responsible for fetching all the strings in the Radix Tree
    // This function, along with "sortRadixTreeAux", are alternatives to the following set of functions:
    //
    // {"heapifyNodes", "heapSortNodes", "sortAndPrintStringsAux"}
    //
    // When used together they do the same functionality, with the addition that "fetchStrings" returns an array of...
    // ...the strings that have been fetched, which may or may not be sorted based on the boolean variable "sort"
    //
    // By default (as can be seen in the function declaration), it sorts them first, unless explicitly set not to
    //
    // On its own, it will fetch the strings of the Radix Tree of root node "t" in whatever order they are in
    //
    // Do not forget to de-allocate the array returned by this function when done using it as done below
    // 
    // // De-allocate the memory allocated for the strings array
    // for (int i = 0; i < numOfStrings; i++) free(strings[i]);
    // free(strings);
    //
    // Has the option to echo output to console - false by default
    //
    char** fetchStringsAux(Node* t, bool echo = false, bool sort = true);
    // Returns pointer to character arrays representing all strings in the Radix Tree of root node "t"
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // To heapify a subtree rooted with node i which is an index in arr. n is size of heap
    //
    void heapifyNodes(Node** arr, int n, int i);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Node sorting function, responsible for sorting "arr" array of nodes of size "size"
    //
    void heapSortNodes(Node** arr, int size);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary string sorting and printing function, responsible for printing all strings in tree of root "t"
    // The function uses the two functions defined previously to sort them alphabetically
    // Has the option to echo output to console - false by default
    //
    void sortAndPrintStringsAux(Node* t, bool echo = false, char* p = 0, int plen = 0);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Node printing function along with its prefix, prints node "n" preceded by its prefix "p", if exists
    // Has the option to echo output to console - false by default
    //
    void printNodeWithPrefix(Node* n, char* p, int pLen, bool echo = false);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary node printing function, responsible for printing all nodes in tree of root node "t"
    // The function uses the function "printNodeWithPrefix" defined above to print individual nodes
    // Has the option to echo output to console - false by default
    //
    void printNodesAux(Node* t, bool echo = false);
    // ---------------------------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------------------------
    // Auxiliary tree printing function, responsible for visualizing the entire tree of root node "t"
    // The tree is generated horizontally and the function works recursively
    // Has the option to echo output to console - false by default
    // By default its output is stored in a text file
    //
    // The function takes three parameters:
    // 1- Node "t" which is the node to be traversed, initialized as the root node of the Radix Tree to be visualized
    // 2- Integer "pLen" which is the length of "p", used in loops responsible for printing out tree components
    // 3- Boolean "echo" which is used in the conditional for printing out to console
    //
    // An example of the final output of the function could be similar to the following:
    //
    // ├───T
    // │   ├───TGGTTGA
    // │   ├───CTT
    // │   └───GCCCC
    // ├───A
    // │   ├───TCGCGAAG
    // │   └───GA
    // ├───CACTAA
    // └───GTACTA
    //
    void printTreeAux(const Node* t, int pLen, bool echo = false);
    // ---------------------------------------------------------------------------------------------------------------

public:

    // Basic constructor, initializes root node to NULL
    RadixTree() : root(0) {};

    // Parameterized constructor, initializes root node to received node
    RadixTree(Node* r) : root(r) {};

    // Copy constructor, creates a clone of the provided Radix Tree by copying all of its nodes recursively
    // It is based on copying the contents of the root node of the original Radix Tree to this one's root node
    // This simple assignment is all we need, since the copy constructor of nodes copies the first child & sibling
    // Consequently, "copying" the first child & sibling leads to them copying their own, recursively copying all nodes
    //
    RadixTree(const RadixTree* orig) { root = (orig->root ? new Node(*orig->root) : 0); };

    // Destructor, responsible for de-allocating memory occupied by Radix Tree
    // Its implementation is similar to the one above, where deleting the root node is responsible for deleting all the other nodes
    //
    ~RadixTree() { delete root; };

    // Publicly usable functions, names self-explanatory
    void addString(char* str);
    void deleteString(char* str);
    bool searchString(char* str);
    int countStrings();
    int countNodes();
    void sortRadixTree();
    char** fetchStrings(bool echo = false, bool sort = true);

    // Printing functions, take the address of the file to print to and a console echo option
    void sortAndPrintStrings(const char* address, bool echo = false);
    void printNodes(const char* address, bool echo = false);
    void printTree(const char* address, bool echo = false);

};

#endif //RADIXTREEPROJECT_RADIXTREE_H