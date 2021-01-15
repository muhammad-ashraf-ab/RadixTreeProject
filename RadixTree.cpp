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
    // i.e. the returned node goes to the root of the tree or somewhere else in it, based on the original function caller
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

    // Create a temporary array to store the current node as well as the link node's characters
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

    return (!t->link) //if it has no children, then it is the end of a string so add 1
        + countStringsAux(t->next) //visit all its siblings 
        + countStringsAux(t->link) //visit all its children
        ;
}

int RadixTree::countNodesAux(Node* t) {
    if (!t) return 0; // if provided tree node "t" is null, then there is no Nodes in it

    return 1 // Always add 1. Doesn't matter if it's a leaf node or not
        + countNodesAux(t->next) //visit all its siblings 
        + countNodesAux(t->link) //visit all its children 
        ;
}

void RadixTree::printNodeAndPrefix(Node* n, char* p, int pLen, bool echo)
{
    if (n->key[0] == 0) {
        nodesFile << "NULL";
        if (echo) cout << "NULL";
    }

    for (int i = 0; i < n->len; i++) {
        nodesFile << n->key[i];
        if (echo) cout << n->key[i];
    }

    nodesFile << ", Prefix: ";
    if (echo) cout << ", Prefix: ";

    for (int i = 0; i < pLen; i++) {
        nodesFile << p[i];
        if (echo) cout << p[i];
    }

    nodesFile << endl;
    if (echo) cout << endl;
}

void RadixTree::printNodesAux(Node* t, bool echo)
{
    static int i = 0;
    static char* prefix = new char[99999];
    static int prefixlen = 0;
    if (t != NULL) {
        nodesFile << "Node #" << i << ' ';
        if (echo) cout << "Node #" << i << ' ';
        printNodeAndPrefix(t, prefix, prefixlen, echo);
        for (int i = 0; i < t->len; i++)
        {
            prefix[prefixlen + i] = t->key[i];
        }
        prefixlen += t->len;
        i++;
        printNodesAux(t->link, echo);
        prefixlen -= t->len;
        printNodesAux(t->next, echo);
    }

}

void RadixTree::printTreeAux(char* p, const RadixTree::Node* node, int pLen, bool echo) {
    if (node != 0) {

        for (int i = 0; i < pLen; i++)
        {
            treeFile << p[i];
            if (echo) cout << p[i];
        }

        treeFile << ((node->next != 0) ? "├───" : "└───");
        if (echo) cout << ((node->next != 0) ? "├───" : "└───");

        for (int i = 0; i < node->len; i++) {
            if (node->key[0] == 0) {
                treeFile << "(NULL)";
                if (echo) cout << "(NULL)";
                break;
            }
            treeFile << node->key[i];
            if (echo) cout << node->key[i];
        }

        treeFile << endl;
        if (echo) cout << endl;

        if (node->next != 0)
            for (int i = 0; i < 4; i++)
                p[pLen + i] = (i ? ' ' : '|');
        else
            for (int i = 0; i < 4; i++)
                p[pLen + i] = ' ';

        printTreeAux(p, node->link, pLen + 4, echo);

        if (node->next == 0) {

            for (int i = 0; i < 4; i++)
                p[pLen + i] = ' ';

            pLen += 4;
        }

        printTreeAux(p, node->next, pLen, echo);
    }
}

void RadixTree::sortNodes(Node** arr, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++)
        {
            if (arr[i]->key[0] > arr[j]->key[0])
            {
                Node* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void RadixTree::printStringsAux(Node* t, char* p, int pLen, bool echo) {
    if (!t) return;
    int itr = 0;
    Node** arr = new Node * [60];
    Node* temp = t;
    while (temp != 0)
    {
        arr[itr] = temp;
        temp = temp->next;
        itr++;
    }
    sortNodes(arr, itr);
    Node* n;
    for (int i = 0; i < itr; i++)
    {
        n = arr[i];
        if (!n->link)
        {
            for (int i = 0; i < pLen; i++)
            {
                segmentsFile << p[i];
                if (echo) cout << p[i];
            }
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
            char* newPrev = new char[n->len + pLen];
            for (int i = 0; i < pLen; i++)
            {
                newPrev[i] = p[i];
            }
            for (int i = 0; i < n->len; i++)
            {
                newPrev[i + pLen] = n->key[i];
            }
            printStringsAux(n->link, newPrev, n->len + pLen, echo);
        }
    }
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
    nodesFile << "Node count:" << countNodes() << endl;
    if (echo) cout << "Node count:" << countNodes() << endl;
    printNodesAux(root, echo);
    nodesFile.close();
}

// Tree visualization function, prints a visualization of the entire radix tree
void RadixTree::printTree(const char* address, bool echo) {
    treeFile.open(address);
    char* prefix = new char[99999];
    prefix[0] = '\0';
    printTreeAux(prefix, root, 0, echo);
    treeFile.close();
}