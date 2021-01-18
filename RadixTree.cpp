//----------------------------------------------------------------------------------------------------------------------
// This project was created for CSE_331 Data Structures And Algorithms course offered in
// Ain Shams University - Faculty of Engineering under the guidance and influence of Dr. Ashraf Abdel Raouf
//
// This implementation has been greatly influenced by the implementation found in the following source:
// https://kukuruku.co/post/radix-trees/
//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "RadixTree.h"

int RadixTree::prefix(char* x, int n, char* key, int m)
{
	// This function iterates over the two character arrays, comparing the value of each character every iteration.
	// This comparison loop will terminate in one of three situations:
	//
	// 1. Iterator reaches end of first character array, meaning all its characters were common. Returns its length.
	// 2. Iterator reaches end of second character array, meaning all its characters were common. Returns its length.
	// 3. Characters are not equal. Returns iterator value at that moment.

	int iterator = 0;

	for (; iterator < n && iterator < m; iterator++)
		if (x[iterator] != key[iterator]) break;

	return iterator;

}

RadixTree::Node* RadixTree::find(Node* t, char* x, int n)
{
	// if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
	// this is consistent with the expected value of the "len" member of each node: number of character, null included

	if (!n) while (x[n++]);

	// if provided tree node "t" is null, then there is nothing to be found, we have reached the end of this branch

	if (!t) return 0;

	// otherwise, if node has value, find the common prefix between it and the key being searched for, "x"
	// the prefix function is provided the size of both character arrays INCLUDING the null character

	int k = prefix(x, n, t->key, t->len);

	// if there's nothing in common, repeat the process for the next node in this tree level

	if (k == 0) return find(t->next, x, n);

	// if all of "x" is prefix, this means the current node IS "x" itself, so return it

	if (k == n) return t;

	// if the entirety of the current node is a prefix itself...
	// this means that the string we're looking for could span more than one node...
	// therefore, continue searching for the remaining part of the searched-for value... how?
	// go to the node that links this tree level to the next level, a.k.a. t->link, and inside it...
	// "x + k" makes us skip ahead the prefix found and search for the rest of the string, meanwhile...
	// "n - k" is the size of the rest of the string.

	if (k == t->len) return find(t->link, x + k, n - k);

	return 0;

}

void RadixTree::split(Node* t, int k)
{
	// Let's use the following example to explain this function:

	// Split the following node at position 4:  [parent ---- "ABCDEF null" ---- child]
	// Required Result:                         [parent ---- "ABCD" ---- "EF null" ---- child]

	// Create a node that carries everything after the first "k" characters in the current node

	Node* p = new Node(t->key + k, t->len - k); // In our example, this means: p = "EF null"

	// Set this newly created node's link/child node as the current one's (maintaining sequentiality)

	p->link = t->link; // In our example, this means: p = "EF null" ---- child

	// Set the current node's link/child as the newly created node

	t->link = p; // In our example, this means: t = "ABCDEF null" ---- "EF null" ---- child

	// Now take the first "k" characters and place them in a temporary character array

	char* a = new char[k];
	for (int i = 0; i < k; i++) a[i] = t->key[i];

	// Delete the current key and replace it with the temporary character array just created, with its size

	delete[] t->key;
	t->key = a;
	t->len = k;

	// In our example, this means: t = "ABCD" ---- "EF null" ---- child
	// Successfully splitting the node at position 4.

}

RadixTree::Node* RadixTree::insert(Node* t, char* x, int n)
{
	// if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
	// this is consistent with the expected value of the "len" member of each node: number of character, null included

	if (!n) while (x[n++]);

	// if current tree node "t" is null, this means we've reached our insertion point, hence create and return the node
	// notice that this function is used either within another function (like addString) or recursively within itself...
	// i.e. the returned node goes to the root of the tree or somewhere else in it, depending on the parent function

	if (!t) return new Node(x, n);

	// otherwise, if node has value, find the common prefix between it and the key to be inserted, "x"
	// the prefix function is provided the size of both character arrays INCLUDING the null character

	int k = prefix(x, n, t->key, t->len);

	// if there's nothing in common, attempt to insert the node to be inserted in the "next" node of the current node

	if (k == 0) t->next = insert(t->next, x, n);

	// otherwise, if part of the current node is a prefix of the key to be added...
	// observe the following examples of current nodes for key ABCF-null:
	// 1. current node: ABCDE-null,    ... k < n (3 < 5)
	// 2. current node: ABC-null,      ... k < n (3 < 5)
	// 3. current node: ABCF,          ... k < n (4 < 5)
	// 4. current node: ABCF-null,     ... k = n (5 = 5) (not in this else if branch)

	else if (k < n)
	{
		// if the node is larger, split it
		// -- this applies to example 1, where "ABCDE-null" would be split into "ABC|DE-null".
		// -- this also applies to example 2, where "ABC-null" would actually be split into "ABC|null"
		// -- however in case of example 3, the current node size is actually equal to the prefix, so no split

		if (k < t->len) split(t, k);

		// at this point, we insert what remains after the prefix (aka the "F")

		t->link = insert(t->link, x + k, n - k);

		// for example 1, this gives us "ABC-DE-null" and "ABC-F-null" as two new children, but no "ABC-null"
		// for example 2, this gives us "ABC-null" and "ABC-F-null" as the two children.
		// for example 3, we only get "ABCF-null"
	}

	// otherwise, the node to be added is the current node itself (k != 0 and >= n), so just return it! (example 4)
	return t;

}

void RadixTree::join(Node* t)
{
	// Let's use the following example to explain this function:

	// Join the following node (with its link): [parent ---- "ABCD" ---- "EF null" ---- child]
	// Required Result:                         [parent ---- "ABCDEF null" ---- child]

	// Point towards the link node of the current node

	Node* p = t->link; // In our example, this means: p = "EF null"

	// Create a character array to store the current node as well as the link node's characters

	char* a = new char[t->len + p->len];

	// Copy the contents of the current node into the temporary array

	for (int i = 0; i < t->len; i++) a[i] = t->key[i]; // a = "ABCD"

	// Copy the contents of the link node afterwards

	for (int i = 0; i < p->len; i++) a[t->len + i] = p->key[i]; // a = "ABCDEF null"

	// Delete the current key and replace it with the temporary character array just created

	delete[] t->key;
	t->key = a;

	// Increase its size by the size of the link node

	t->len += p->len;

	// Set the link of the current node as the link of its own link node (i.e. skipping it)

	t->link = p->link; // In our example, this means: t = "ABCDEF null" ---- child

	// Finally, delete the now-duplicated node
	delete p;

}

RadixTree::Node* RadixTree::remove(Node* t, char* x, int n)
{
	// if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
	// this is consistent with the expected value of the "len" member of each node: number of character, null included

	if (!n) while (x[n++]);

	// if provided tree node "t" is null, then there is nothing to be removed

	if (!t) return 0;

	// otherwise, if node has value, find the common prefix between it and the key being searched for, "x"
	// the prefix function is provided the size of both character arrays INCLUDING the null character

	int k = prefix(x, n, t->key, t->len);

	// if all of "x" is prefix, this means the current node IS "x" itself, so remove it (by replacing it with its next)

	if (k == n)
	{
		Node* p = t->next;
		delete t;
		return p;
	}

	// if there's nothing in common, repeat the process for the next node in this tree level

	if (k == 0) t->next = remove(t->next, x, n);

	// otherwise if the current node is a prefix itself... for example...
	// key: ABCDE-null
	// current node: ABC

	// then: k = 3, t->len = 3, n = 6

	else if (k == t->len)
	{
		// proceed to the link(s) of this node with the rest of the key (in this case, DE-null), attempting to remove again
		// let's say the link was "DE-null". If we go upwards, we find that this is the (k == n) case, where the link gets...
		// ...replaced by the link after it (connected to it by "next").

		t->link = remove(t->link, x + k, n - k);

		// accordingly, if "t" ends up with only one link (which we can find by seeing if its link has a next or not)...
		// ...merge it with that link so as to make it one node.

		if (t->link && !t->link->next) join(t);
	}

	// otherwise, the node to be removed was not found, so return this node as it is (i.e. no changes)

	return t;

}

int RadixTree::countStringsAux(Node* t) {

	if (!t) return 0; // if provided tree node "t" is null, then there is no strings in it

	return (!t->link) // if it has no children, then it is the end of a string so add 1
		+ countStringsAux(t->next) // then recursively visit all siblings
		+ countStringsAux(t->link) // then recursively visit all children
		;
}

int RadixTree::countNodesAux(Node* t) {

	if (!t) return 0; // if provided tree node "t" is null, then there is no Nodes in it

	return 1 // Always add 1. Doesn't matter if it's a leaf node or not
		+ countNodesAux(t->next) // then recursively visit all siblings 
		+ countNodesAux(t->link) // then recursively visit all children 
		;
}

void RadixTree::printNodeAndPrefix(Node* n, char* p, int pLen, bool echo)
{

	// First we check if node has a prefix, because if so, we print it, followed by a separator

	if (pLen) {
		for (int i = 0; i < pLen; i++) {
			nodesFile << p[i];
			if (echo) cout << p[i];
		}
		nodesFile << " | ";
		if (echo) cout << " | ";
	}

	// Then we check if this is an empty node (only contains null character to determine the end of a segment) and if so we print NULL and terminate function

	if (n->key[0] == 0) {
		nodesFile << "(NULL)\n";
		if (echo) cout << "(NULL)\n";
		return;
	}

	// Otherwise if it is not empty, we print its key, character by character, since some nodes may not have a null terminator

	for (int i = 0; i < n->len; i++) {
		nodesFile << n->key[i];
		if (echo) cout << n->key[i];
	}

	// And finally a line break

	nodesFile << endl;
	if (echo) cout << endl;

}

void RadixTree::printNodesAux(Node* t, bool echo)
{
	static int v; // The number of nodes already visited
	static char* p; // Character array to contain prefix
	static int pLen; // The length of prefix

	// If commencing a new test, which can be concluded if current node is the root, reset "v"

	if (t == root) v = 0;

	nodesFile << "Node #" << v << " - "; // Printing for the output file
	if (echo) cout << "Node #" << v << " - "; // Printing for the output in the terminal
	printNodeAndPrefix(t, p, pLen, echo); // Printing the node and its prefix

	// The following loop adds the key of the current node t to the prefix array
	// It also simultaneously increments the variable representing prefix length, "pLen", to represent this change
	// Of course, before addition, the character array needs to be resized

	p = (char*) realloc(p, sizeof(char) * (pLen + t->len + 1));
	for (int i = 0; i < t->len; i++)
		p[pLen++] = t->key[i];

	p[pLen] = 0;

	v++; // Incrementing the visited nodes counter
	if (t->link != 0) printNodesAux(t->link, echo); // Recursive call to visit all the descendents of this node, if exists
	pLen -= t->len; // Decrementing "pLen" to remove the prefix of the current node "t", representing sibling transition
	if (t->next != 0) printNodesAux(t->next, echo); // Recursive call to visit all the siblings of this node, if exists
	if (t == root) { // If pointers match... (i.e. "t" is indeed the root node)

		free(p); // Finally, once all recursive calls are done and node printing is over, free the memory allocated for static "p"
		p = (char*) calloc(0, 0); // Then re-allocate it with minimum memory for potential future re-usage of static "p"

	}

}

void RadixTree::printTreeAux(const Node* node, int pLen, bool echo) {

	// Allocate two character arrays with one character each, "p" and "temp".
	// "p" will be used in storing special character sequences to be used in the tree visualization process.
	// "temp" will be used in dynamically resizing "p", mechanism explained further down in this function's code.

	static char* p, * temp;

	{	// This part is in a scope of its own since it is only concerned with printing and does not affect "p" or "temp"

		// This loop is responsible for printing out the contents of p. It always starts printing its content
		// prior to other operations; hence why p is also referred to as The Prefix.
		for (int i = 0; i < pLen; i++)
		{
			// Prints to the tree file.
			treeFile << p[i];

			// Prints to the console, given echo is true.
			if (echo) cout << p[i];
		}

		// This line prints a special set of characters that represent whether the node has next siblings or it is the
		// last sibling. It should be noted, however, that the special characters sometimes do not appear as intended
		// in Windows console. If this happens, the specialized console of some IDEs is recommended for better
		// visualization. Otherwise, the text file should provide better visualization, given right encoding.
		//
		// Example of the different evaluations for the conditions:
		// ...
		// ├───TGGTTGA
		// ├───CTT
		// └───GCCCC
		treeFile << ((node->next != 0) ? "├───" : "└───");

		// Prints to the console, given echo is true.
		if (echo) cout << ((node->next != 0) ? "├───" : "└───");

		// Loop responsible for printing out the key of a node. We don't simply make it print the pointer, since not all nodes have a null terminator.
		for (int i = 0; i < node->len; i++) {

			// Checks if the key for the node is null. If it is, prints (NULL) to prevent empty tree branches.
			if (node->key[0] == 0) {

				// Prints to the tree file.
				treeFile << "(NULL)";

				// Prints to the console, given echo is true.
				if (echo) cout << "(NULL)";

				// Breaks outside the loop to prevent printing gibberish.
				break;
			}

			// Prints the content of the node, including the final null character if it exists, given first was not null.
			treeFile << node->key[i];

			// Prints to the console, given echo is true
			if (echo) cout << node->key[i];
		}

		// Prints a new line
		treeFile << endl;
		if (echo) cout << endl;

	}

	// The following part is responsible for appending the special character set "│   " to branches of the tree.
	// Otherwise, it only prints four white spaces like "    ".
	//
	// It should also be noted that the ASCII character 124 (|) was used here due to limitations of char datatype.
	// The ASCII character 179 (│) can be used instead if this code is implemented using strings for a better looking tree.
	//
	//
	// Example of the final output:
	// 1- In case of adding a bar:
	//
	// ...
	// ├───T
	// │   ├───CTT
	// ...
	//
	// 2- In case of adding a space:
	//
	// ...
	// │       │       └───A
	// │       └───G
	// ...
	//
	// Of course, the size of the character array needs to be increased by 4 units to be able to have those characters added.
	// The eight following lines of code cover the procedure of increasing the size of the prefix while maintaining its data.
	// An example explaining how they operate in this recursive environment can be found at the end of this file.

	// Allocate memory for the temporary character array for "pLen" characters for the prefix.
	temp = new char[pLen];

	// Copy the content that currently exists in the prefix into the temporary array.
	for (int i = 0; i < pLen; i++) temp[i] = p[i];

	// Delete whatever the prefix is pointing at currently.
	delete[] p;

	// Now expand the prefix; 4 characters for the data.
	p = new char[pLen + 4];

	// Now fill it with its original value + the new special character set to be added, which is chosen based on existence of a sibling.
	for (int i = 0; i < pLen + 4; i++) p[i] = (i < pLen) ? temp[i] : (node->next != 0 ? (i - pLen ? ' ' : '|') : ' ');

	// Finally, de-allocate the temporary array.
	delete[] temp;

	// Now, make a recursive call to the function to traverse on to the next link using the new content of p.
	// It should be noted that the size is sent as pLen + 4 to keep the original content of p after returning,
	// while sending the child the full prefix (i.e. the old prefix + what we just appended to it) as it is in the next level of the tree.
	// This is provided that a child does exist. If not, resume function execution.
	if (node->link != 0) printTreeAux(node->link, pLen + 4, echo);

	// Once done with all children and their siblings, we return to this function with the prefix length associated with it.
	// So now, we make a recursive call to next sibling of the node to keep iterating through them while maintaining the same prefix.
	// This is provided that a sibling does exist. If not, resume function execution.
	if (node->next != 0) printTreeAux(node->next, pLen, echo);

	// Finally, check if at the very first function call (via checking if node is root). If so, nullify the prefix.
	if (node == root) { delete[] p; p = new char[0]; }

	// Afterwards the function terminates and the stack is popped (i.e. parent function may now resume execution).
	// An example of the final output of the function can be found in the function declaration in RadixTree.h

}

void RadixTree::heapifyNodes(Node** arr, int n, int i)
{
	int largest = i; // Initialize largest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

	// If left child is larger than root
	if (l < n && arr[l]->key[0] > arr[largest]->key[0])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r]->key[0] > arr[largest]->key[0])
		largest = r;

	// If largest is not root
	if (largest != i) {

		swap(arr[i], arr[largest]);

		// Recursively heapify the affected sub-tree
		heapifyNodes(arr, n, largest);
	}
}

void RadixTree::heapSortNodes(Node** arr, int size)
{
	// Build heap (rearrange array)
	for (int i = size / 2 - 1; i >= 0; i--)
		heapifyNodes(arr, size, i);

	// One by one extract an element from heap
	for (int i = size - 1; i > 0; i--) {
		// Move current root to end
		Node* temp = arr[i];
		arr[i] = arr[0];
		arr[0] = temp;

		// call max heapify on the reduced heap
		heapifyNodes(arr, i, 0);
	}
}

void RadixTree::printStringsAux(Node* t, char* p, int pLen, bool echo) {

	if (!t) return; // if provided tree node "t" is null, then there is nothing to do.

	int itr = 0;  // iterator to loop over all siblings of the current node.

	// Array of nodes to hold all nodes in the current level.
	// Size equal the number of characters in ASCII table.
	Node** arr = new Node * [128];
	Node* temp = t;

	// Store all siblings in the array.
	while (temp != 0)
	{
		arr[itr] = temp;
		temp = temp->next;
		itr++;
	}

	// Sort using heap sort algorithm.
	heapSortNodes(arr, itr);
	Node* n;

	// Looping over the sorted array of nodes
	for (int i = 0; i < itr; i++)
	{
		n = arr[i];

		// If the node has no children, print it.
		if (!n->link)
		{
			// Printing the prefix of the node
			for (int i = 0; i < pLen; i++)
			{
				segmentsFile << p[i];
				if (echo) cout << p[i];
			}

			// Printing the the node itself
			for (int i = 0; i < n->len; i++)
			{
				segmentsFile << n->key[i];
				if (echo) cout << n->key[i];
			}
			segmentsFile << endl;
			if (echo) cout << endl;
		}
		else
		{
			// If the node has children, create a new prefix that contains the previous prefix and the current node.
			char* newPrefix = new char[n->len + pLen];

			// Inserting the characters of the node's prefix.
			for (int i = 0; i < pLen; i++)
				newPrefix[i] = p[i];

			// Inserting the characters of the node itself.
			for (int i = 0; i < n->len; i++)
				newPrefix[i + pLen] = n->key[i];

			// Visit current node's children and pass the newly created prefix.
			printStringsAux(n->link, newPrefix, n->len + pLen, echo);

			// Then deallocate its memory when done.
			delete[] newPrefix;
		}
	}

	// De-allocate array of nodes.
	delete[] arr;
}

// Destructor, de-allocates memory occupied by radix tree by deleting all its nodes recursively
RadixTree::~RadixTree() {

	RadixTree* rt;

	// If root has a child, create a Radix Tree whose root is that child and attempt to delete it
	// An attempt to delete a Radix Tree calls *this* destructor, effectively recursively going to the last level's first child
	// Once the last level's first child is reached, this if condition evaluates to false and this destructor proceeds in operation
	if (root->link != 0) { rt = new RadixTree(root->link); delete rt; }

	// It now does the same but for siblings rather than children till it reaches the last sibling, then proceeds in operation
	if (root->next != 0) { rt = new RadixTree(root->next); delete rt; }

	// It now deletes this node and pops the stack, allowing the previous destructor (aka the previous sibling) to get deleted
	// Once all siblings of a certain leaf are deleted, they allow the destructor of their parent to proceed
	// That parent then proceeds to traverse through its siblings, repeating the process for each
	delete root;

}

// Addition function, updates root with a new root containing the string to be added
void RadixTree::addString(char* str) {
	root = insert(root, str);
}

// Deletion function, updates root with a new root that does not contain the string to be deleted
void RadixTree::deleteString(char* str) {
	root = remove(root, str);
}

// Searching function returns, boolean value based on the result of the finder function
bool RadixTree::searchString(char* str) {
	return find(root, str) != 0;
}

// Counting strings function, returns the total number of string in the current Radix Tree
int RadixTree::countStrings() {
	return countStringsAux(root);
}

// Counting nodes function, returns the total number of nodes in the current Radix Tree
int RadixTree::countNodes() {
	return countNodesAux(root);
}

// Strings printing function, prints strings in tree sorted in alphabetical order
void RadixTree::printStrings(const char* address, bool echo)
{
	segmentsFile.open(address);
	printStringsAux(root, 0, 0, echo);
	segmentsFile.close();
}

// Node printing function, prints node count, individual nodes, and their respective prefixes
void RadixTree::printNodes(const char* address, bool echo) {
	nodesFile.open(address);
	nodesFile << "Node Count: " << countNodes() << endl;
	if (echo) cout << "Node Count: " << countNodes() << endl;
	nodesFile << "Note: For nodes with prefixes, the prefix is printed before the node key and they are separated by the \"|\" character.\n\n";
	if (echo) cout << "Note: For nodes with prefixes, the prefix is printed before the node key and they are separated by the \"|\" character.\n\n";
	printNodesAux(root, echo);
	nodesFile.close();
}

// Tree visualization function, prints a visualization of the entire radix tree
void RadixTree::printTree(const char* address, bool echo) {
	treeFile.open(address);
	// The auxiliary tree printing function takes different inputs:
	// The root node of the current Radix Tree and the prefix data length (where the prefix in the tree is a special set of characters, explained later)
	printTreeAux(root, 0, echo);
	treeFile.close();
}

/*  ┌ Example on Recursive Tree Printing with Dynamic Memory Allocation ┐
┌───┴───────────────────────────────────────────────────────────────────┴────────────────────────────────────────┐
│																												 │
│  A test that matches the assumptions made in this example can be generated using the following setup:          │
│  ─ Randomization Seed: 1337																					 │
│  ─ Min. Segment length: 4																						 │
│  ─ Max. Segment length: 7																						 │
│  ─ Number of segments: 10																						 │
│																												 │
│  Upon executing "printTreeAux", first, we have pLen set at 0, and "static char* p, * temp;" which gives us:	 │
│  p ──> [], temp ──> [], pLen = 0																				 │
│																												 │
│  They will not be instantiated again for the lifetime of this program.										 │
│  Now let's proceed with the function, skip the part concerning printing...									 │
│																												 │
│  "temp" is resized to have size pLen (= 0 at this step)														 │
│  temp ──> [?], pLen = 0																						 │
│																												 │
│  The loop sets the first "pLen" characters in "temp" as "p" (= 0 at this step, i.e. no changes), so:			 │
│  temp ──> [?], pLen = 0																						 │
│																												 │
│  At this point, "p" is no longer needed, so we de─allocate its memory											 │
│  p ──> garbage, pLen = 0																						 │
│																												 │
│  Now we allocate "pLen + 4 (= 4 at this step)" characters for p												 │
│  p ──> [?][?][?][?], pLen = 0																					 │
│																												 │
│  Next, we copy "pLen" characters from "temp" into "p" then add "|   " or "    " according to the situation	 │
│  p ──> [|][ ][ ][ ], pLen = 0																					 │
│																												 │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory									 │
│  temp ──> garbage, pLen = 0																					 │
│																												 │
│  The value of pLen makes sense at this point, since for this function (the root function), there's no prefix	 │
│  That is because root nodes have no prefix tree symbols before the "├───"										 │
│  Notice how the prefix itself, however, is actually of length 4												 │
│  Hence, upon proceeding to a child, we send "pLen + 4"														 │
│																												 │
├────────────────────────────────── Assuming a child does exist, we will proceed ────────────────────────────────┤
├──────────────────── Here we recurse, sending the function the first child and pLen + 4 (= 4) ──────────────────┤
│																												 │
│  First, we now have:																							 │
│  p ──> [|][ ][ ][ ], temp ──> garbage, pLen = 4 (which means those 4 characters get printed)					 │
│																												 │
│  We now allocate new memory for the "temp" array with size pLen (= 4 at this step)							 │
│  temp ──> [?][?][?][?], pLen = 4																				 │
│																												 │
│  The loop sets the first "pLen" characters in "temp" as "p", so:												 │
│  temp ──> [|][ ][ ][ ], pLen = 4																				 │
│																												 │
│  At this point, "p" is no longer needed, so we de─allocate its memory											 │
│  temp ──> garbage, pLen = 4																					 │
│																												 │
│  Next we allocate "pLen + 4 (= 8 at this step)" characters for p												 │
│  p ──> [?][?][?][?][?][?][?][?], pLen = 4																		 │
│																												 │
│  Next, we copy "pLen" characters from "temp" into "p" then add "|   " or "    " according to the next node	 │
│  If next node exists, we "|   ", otherwise we add "   "... This will be further explained later				 │
│  Let's assume the next node (sibling node) exists, so...														 │
│  p ──> [|][ ][ ][ ][|][ ][ ][ ], pLen = 4																		 │
│																												 │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory									 │
│  temp ──> garbage, pLen = 4																					 │
│																												 │
│  Again, the value of pLen for this function makes sense, as this is the root's child, i.e. 2nd tree level		 │
│																												 │
│  ───────────── Assuming there is no child, the function skips the first recursive function call ───────────────┤
│  ───────────────────────── Following the assumption made above, there's a next node ───────────────────────────┤
│  ────────── Therefore we recurse again, sending the function the next node and the same pLen (= 4) ────────────┤
│																												 │
│  First, we now have:																							 │
│  p ──> [|][ ][ ][ ][|][ ][ ][ ], temp ──> garbage, pLen = 4													 │
│																												 │
│  Same as before, allocate memory for "temp" of size "pLen " then copy "pLen" characters from "p"				 │
│  temp ──> [?][?][?][?], pLen = 4																				 │
│  temp ──> [|][ ][ ][ ], pLen = 4																				 │
│																												 │
│  Then we delete "p", allocate "pLen + 4 " characters, copy all of "temp" then add "|   " or "    "			 │
│  p ──> garbage, pLen = 4																						 │
│  p ──> [?][?][?][?][?][?][?][?], pLen = 4																		 │
│  p ──> [|][ ][ ][ ][|][ ][ ][ ], pLen = 4																		 │
│																												 │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory									 │
│  temp ──> garbage, pLen = 4																					 │
│																												 │
│  ─────────── Let's assume again that there's no child, and skip the first recursive function call ─────────────┤
│  ───────────── Assuming there is a next node, we recurse again with it, and the same pLen (= 4) ───────────────┤
│																												 │
│  The same exact execution as the previous non─empty function is done here again...							 │
│  ...except for one thing: Let's assume this node has no next node, meaning that...							 │
│  ...when choosing which special character set to append to the prefix, we'll go...							 │
│  ...with the four spaces, the following example should show both choices in action							 │
│																												 │
│          .																									 │
│          .																									 │
│          .																									 │
│																		 										 │
│  │   │   ├───TC... <── Choosing "│   "																		 │
│  │   │   └───AT... <── Choosing "│   "																		 │
│  │   └───GCC																		 						 	 │
│  │       ├───AA... <── Choosing "    "																		 │
│  │       └───GT... <── Choosing "    "																		 │
│  ├───T																		 								 │
│  │   ├───ATCTGT... <── Choosing "│   "																		 │
│  │   ├───CATAAC... <── Choosing "│   "																		 │
│																		 										 │
│          .																									 │
│          .																									 │
│          .																									 │
│																												 │
│  i.e. If there's no next node where there could potentially be children										 │
│																												 │
│  After the current function is over, we will have the following values:										 │
│																												 │
│  p ──> [|][ ][ ][ ][ ][ ][ ][ ], temp ──> garbage, pLen = 4													 │
│																												 │
│  This function then terminates, popping the stack and allowing its parent function to continue execution		 │
│  Its parent (@pLen = 0) was waiting for its first recursive call to finish, and now commences the second one	 │
│  Since we had assumed it had a sibling node, it will go through the second recursive call						 │
│																												 │
│  This recursive cycle repeats until all siblings and children have been visited and their prefixes determined	 │
│																												 │
└────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
*/