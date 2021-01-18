//----------------------------------------------------------------------------------------------------------------------
// This project was created for CSE_331 Data Structures And Algorithms course offered in
// Ain Shams University - Faculty of Engineering under the guidance and influence of Dr. Ashraf Abdel Raouf
//
// This implementation has been greatly influenced by the implementation found in the following source:
// https://kukuruku.co/post/radix-trees/
//----------------------------------------------------------------------------------------------------------------------
#ifndef RADIXTREEPROJECT_RADIXTREE_H
#define RADIXTREEPROJECT_RADIXTREE_H
#include <fstream>
using namespace std;

class RadixTree {
private:

	// Radix Tree's private inner class: Node
	class Node {
	public:

		// The following two pointers are the core operational linker nodes of the radix tree

		// ---- The "next" node. This resembles a "sibling" node to the current node, meaning that it is at the same tree level
		Node* next;

		// ---- The "link" node. This resembles a "child" node to the current node from the next tree level, not necessarily the only child
		Node* link;

		// The value of the node
		char* key;

		// Number of characters in the node (includes the null character - if it exists)
		int len;

		// Basic constructor, initializes the node members as follows:
		// -- Node length:  n
		// -- Link node:    NULL
		// -- Next node:    NULL
		// -- Node value:   Loop sets character array
		Node(const char* x, int n) : next(0), link(0), len(n)
		{
			key = new char[len];
			for (int i = 0; i < len; i++) key[i] = x[i];
		}

		// Copy constructor, makes this node an exact copy of node "orig"
		Node(const Node* orig) : next(orig->next), link(orig->link), len(orig->len)
		{
			key = new char[len];
			for (int i = 0; i < len; i++) key[i] = orig->key[i];
		}

		// Equality operator overloading
		bool operator==(const Node& rhs)
		{
			if (len != rhs.len) return false; // Compare lengths first, if they do not match then they can't be equal therefore return false
			for (int i = 0; i < len; i++) if (key[i] != rhs.key[i]) return false; // Next, compare each character, on conflict return false
			if (*link != *(rhs.link) || *next != *(rhs.next)) return false; // Finally, compare if its first child or sibling are the same
			// Notice that the check above recursively checks all of the children and siblings of these two nodes - aka sub-tree checking
			return true; // If comparison reaches this line then the two nodes are indeed congruent to each other, therefore return true
		}

		// Inequality operator overloading - literally the opposite of the equality operator
		bool operator!=(const Node& rhs) { return !(*this == rhs); }

		// Basic deconstructor, de-allocates memory occupied by node's key
		~Node() { delete[] key; }

	};

	// Radix Tree's root node
	Node* root;

	// File streams to print to respective files
	ofstream segmentsFile;
	ofstream nodesFile;
	ofstream treeFile;

	// ----------------------------------------------------------------------------------------
	// Prefix function, responsible for comparing two character arrays "x" and "key",
	// of lengths "n" and "m" respectively.
	//
	int prefix(char* x, int n, char* key, int m);
	// Returns the number of common prefix characters
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Finder function, responsible for finding key "x" in tree of root node "t"
	//
	Node* find(Node* t, char* x, int n = 0);
	// Returns pointer to the node corresponding to "x", if found
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Splitting function, responsible for splitting a node into two at "k"
	// Used as part of the insertion process
	//
	void split(Node* t, int k);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Insertion function, responsible for inserting a node in its right position
	//
	Node* insert(Node* t, char* x, int n = 0);
	// Returns pointer to the inserted node
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Joining function, responsible for joining a node with its link
	// Used as part of the removal process
	//
	void join(Node* t);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Removal function, responsible for removing key "x" in tree of root node "t"
	//
	Node* remove(Node* t, char* x, int n = 0);
	// Returns pointer to the node that takes place of removed node
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// String counting function, responsible for counting strings in tree of root node "t"
	//
	int countStringsAux(Node* t);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Node counting function, responsible for counting nodes in tree of root node "t"
	//
	int countNodesAux(Node* t);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Single node and prefix printing function, prints node "n" and its prefix "p"
	// Has the option to echo output to console - false by default
	//
	void printNodeAndPrefix(Node* n, char* p, int pLen, bool echo = false);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Node printing function, responsible for printing all nodes in tree of root node "t"
	// The function uses the printNodeAndPrefix function defined previously to print them
	// Has the option to echo output to console - false by default
	//
	void printNodesAux(Node* t, bool echo = false);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Tree printing function, responsible for visualizing the entire tree of root node "t"
	// Has the option to echo output to console - false by default
	// Function outputs into the Trees.txt file. The tree is generated horizontally and the function works recursively.
	//
	// The function takes four parameters:
	// 1- The char array p is responsible for printing white spaces and branches in the tree structure
	// 2- The Node node is the current node to be traversed
	// 3- The int pLen is the length of p. It is used for loops responsible for printing out contents of p
	// 4- The bool echo is used in the conditional for printing out to console
	//
	// An example of the final output of the function could be similar to the following:
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
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// To heapify a subtree rooted with node i which is an index in arr. n is size of heap
	//
	void heapifyNodes(Node** arr, int n, int i);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Node sorting function, responsible for sorting "arr" array of nodes of size "size"
	//
	void heapSortNodes(Node** arr, int size);
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------
	// Strings printing function, responsible for printing all strings in tree of root "t"
	// The function uses the sortNodes function defined previously to sort them alphabetically
	// Has the option to echo output to console - false by default
	//
	void printStringsAux(Node* t, char* p, int plen, bool echo = false);
	// ----------------------------------------------------------------------------------------

public:

	// Basic constructor, initializes root node to NULL
	RadixTree() : root(0) {};

	// Parameterized constructor, initializes root node to received node
	RadixTree(Node* r) : root(r) {};

	// Destructor, responsible for de-allocating memory occupied by Radix Tree
	~RadixTree();

	// Publicly usable functions, names self-explanatory
	void addString(char* str);
	void deleteString(char* str);
	bool searchString(char* str);
	int countStrings();
	int countNodes();

	// Printing functions, take the address of the file to print to and a console echo option
	void printStrings(const char* address, bool echo = false);
	void printNodes(const char* address, bool echo = false);
	void printTree(const char* address, bool echo = false);

};

#endif //RADIXTREEPROJECT_RADIXTREE_H